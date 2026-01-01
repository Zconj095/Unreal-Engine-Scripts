#include "NeuralCalibrationActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralCalibrationActor::ANeuralCalibrationActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralCalibrationActor::BeginPlay()
{
    Super::BeginPlay();

    InitializeWeights();

    TArray<FVector> Inputs;
    TArray<FVector> Outputs;

    Inputs.Add(FVector(0.1f, 0.2f, 0.3f));
    Inputs.Add(FVector(0.2f, 0.3f, 0.4f));
    Inputs.Add(FVector(0.3f, 0.4f, 0.5f));

    Outputs.Add(FVector(0.2f, 0.4f, 0.6f));
    Outputs.Add(FVector(0.4f, 0.6f, 0.8f));
    Outputs.Add(FVector(0.6f, 0.8f, 1.0f));

    Train(Inputs, Outputs, 1000);

    FVector Prediction = Predict(FVector(0.15f, 0.25f, 0.35f));
    UE_LOG(LogTemp, Log, TEXT("Predicted Adjustment Magnitude: [%.4f, %.4f, %.4f]"), Prediction.X, Prediction.Y, Prediction.Z);
}

void ANeuralCalibrationActor::Train(const TArray<FVector>& Inputs, const TArray<FVector>& Outputs, int32 Epochs, float LearningRate)
{
    if (Inputs.Num() != Outputs.Num() || Inputs.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Input and output datasets must be non-empty and matching in size."));
        return;
    }

    for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
    {
        float EpochError = 0.f;

        for (int32 SampleIndex = 0; SampleIndex < Inputs.Num(); ++SampleIndex)
        {
            FVector Predicted = Predict(Inputs[SampleIndex]);
            FVector ErrorVec = Outputs[SampleIndex] - Predicted;

            EpochError += ErrorVec.SizeSquared();

            for (int32 Dim = 0; Dim < 3; ++Dim)
            {
                float PredictionValue = Predicted[Dim];
                float Delta = ErrorVec[Dim] * PredictionValue * (1.f - PredictionValue);
                FVector& WeightRow = HiddenLayerWeights[Dim];
                WeightRow += LearningRate * Delta * Inputs[SampleIndex];
                Biases[Dim] += LearningRate * Delta;
            }
        }

        if (Epoch % 100 == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d: Error = %.6f"), Epoch + 1, Epochs, EpochError);
        }
    }
}

FVector ANeuralCalibrationActor::Predict(const FVector& Delta) const
{
    FVector Result;
    for (int32 Dim = 0; Dim < 3; ++Dim)
    {
        float DotProduct = HiddenLayerWeights[Dim] | Delta;
        float Activation = DotProduct + Biases[Dim];
        Result[Dim] = Sigmoid(Activation);
    }
    return Result;
}

float ANeuralCalibrationActor::Sigmoid(float Value)
{
    return 1.f / (1.f + FMath::Exp(-Value));
}

void ANeuralCalibrationActor::InitializeWeights()
{
    HiddenLayerWeights.SetNum(3);
    Biases.SetNumZeroed(3);

    for (int32 Index = 0; Index < 3; ++Index)
    {
        HiddenLayerWeights[Index] = FVector(
            FMath::FRandRange(-0.5f, 0.5f),
            FMath::FRandRange(-0.5f, 0.5f),
            FMath::FRandRange(-0.5f, 0.5f)
        );
    }
}
