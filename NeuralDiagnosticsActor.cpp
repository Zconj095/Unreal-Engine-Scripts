#include "NeuralDiagnosticsActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralDiagnosticsActor::ANeuralDiagnosticsActor()
{
    PrimaryActorTick.bCanEverTick = false;
    OutputWeights.SetNum(3);
    Biases.Init(0.f, 3);

    for (int32 Index = 0; Index < 3; ++Index)
    {
        OutputWeights[Index] = FVector(
            FMath::FRandRange(-0.5f, 0.5f),
            FMath::FRandRange(-0.5f, 0.5f),
            FMath::FRandRange(-0.5f, 0.5f)
        );
    }
}

void ANeuralDiagnosticsActor::BeginPlay()
{
    Super::BeginPlay();

    TArray<FVector> Inputs;
    TArray<FVector> Targets;
    PopulateDefaultData(Inputs, Targets);

    float AverageError = Evaluate(Inputs, Targets);
    UE_LOG(LogTemp, Log, TEXT("Neural diagnostics average error rate: %.6f"), AverageError);
}

float ANeuralDiagnosticsActor::Evaluate(const TArray<FVector>& Inputs, const TArray<FVector>& Targets)
{
    if (Inputs.Num() == 0 || Inputs.Num() != Targets.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Inputs and targets must be populated and the same size."));
        return -1.f;
    }

    TArray<FVector> Predictions;
    Predictions.Reserve(Inputs.Num());

    for (const FVector& Sample : Inputs)
    {
        Predictions.Add(Predict(Sample));
    }

    return CalculateAverageError(Predictions, Targets);
}

FVector ANeuralDiagnosticsActor::Predict(const FVector& Input) const
{
    FVector Result;

    for (int32 Dim = 0; Dim < 3; ++Dim)
    {
        float DotProduct = OutputWeights[Dim] | Input;
        float Activation = DotProduct + Biases[Dim];
        Result[Dim] = Sigmoid(Activation);
    }

    return Result;
}

float ANeuralDiagnosticsActor::CalculateAverageError(const TArray<FVector>& Predictions, const TArray<FVector>& Targets) const
{
    double Sum = 0.0;

    for (int32 Index = 0; Index < Predictions.Num(); ++Index)
    {
        FVector Delta = Predictions[Index] - Targets[Index];
        Sum += Delta.SizeSquared();
    }

    return static_cast<float>(Sum / Predictions.Num());
}

float ANeuralDiagnosticsActor::Sigmoid(float Value)
{
    return 1.f / (1.f + FMath::Exp(-Value));
}

void ANeuralDiagnosticsActor::PopulateDefaultData(TArray<FVector>& Inputs, TArray<FVector>& Targets) const
{
    Inputs =
    {
        FVector(0.1f, 0.2f, 0.3f),
        FVector(0.4f, 0.5f, 0.6f),
        FVector(0.7f, 0.8f, 0.9f)
    };

    Targets =
    {
        FVector(0.15f, 0.25f, 0.35f),
        FVector(0.45f, 0.55f, 0.65f),
        FVector(0.75f, 0.85f, 0.95f)
    };
}
