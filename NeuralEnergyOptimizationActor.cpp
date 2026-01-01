#include "NeuralEnergyOptimizationActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEnergyOptimizationActor::ANeuralEnergyOptimizationActor()
{
    PrimaryActorTick.bCanEverTick = false;
    Weights = {0.002f, 0.0015f, 0.001f};
}

void ANeuralEnergyOptimizationActor::BeginPlay()
{
    Super::BeginPlay();

    TArray<FVector> Inputs =
    {
        FVector(500.f, 200.f, 100.f),
        FVector(600.f, 250.f, 150.f),
        FVector(400.f, 180.f, 120.f)
    };

    TArray<float> Targets = {400.f, 500.f, 340.f};
    Train(Inputs, Targets, 1000);

    float Prediction = Predict(700.f, 300.f, 200.f);
    UE_LOG(LogTemp, Log, TEXT("Predicted Energy Change: %.4f J"), Prediction);
}

void ANeuralEnergyOptimizationActor::Train(const TArray<FVector>& Inputs, const TArray<float>& Targets, int32 Epochs, float LearningRate)
{
    if (Inputs.Num() != Targets.Num() || Inputs.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid training data."));
        return;
    }

    for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
    {
        float EpochError = 0.f;
        for (int32 SampleIndex = 0; SampleIndex < Inputs.Num(); ++SampleIndex)
        {
            float Prediction = Predict(Inputs[SampleIndex].X, Inputs[SampleIndex].Y, Inputs[SampleIndex].Z);
            float Error = Targets[SampleIndex] - Prediction;
            EpochError += FMath::Square(Error);

            for (int32 Dim = 0; Dim < Weights.Num(); ++Dim)
            {
                Weights[Dim] += LearningRate * Error * Inputs[SampleIndex][Dim];
            }

            Bias += LearningRate * Error;
        }

        if (Epoch % 200 == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d: Error=%.6f"), Epoch + 1, Epochs, EpochError);
        }
    }
}

float ANeuralEnergyOptimizationActor::Predict(float HeatTransfer, float WorkDone, float MagicalContribution) const
{
    if (Weights.Num() < 3)
    {
        return 0.f;
    }

    float Value = HeatTransfer * Weights[0] + WorkDone * Weights[1] + MagicalContribution * Weights[2] + Bias;
    return Sigmoid(Value) * 1000.f; // scale to match expected magnitude
}

float ANeuralEnergyOptimizationActor::Sigmoid(float Value)
{
    return 1.f / (1.f + FMath::Exp(-Value));
}
