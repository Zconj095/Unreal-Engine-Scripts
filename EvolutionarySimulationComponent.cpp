#include "EvolutionarySimulationComponent.h"
#include "Math/UnrealMathUtility.h"

UEvolutionarySimulationComponent::UEvolutionarySimulationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    PatternSize = 4;
    TemporalSize = 3;
}

void UEvolutionarySimulationComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeRandomVectors();
    TArray<double> Result = ComputeEvolutionaryPotential();
    FString StringValue = TEXT("");
    for (double Value : Result)
    {
        StringValue += FString::SanitizeFloat(Value) + TEXT(" ");
    }
    UE_LOG(LogTemp, Log, TEXT("Evolutionary Simulation Potential (P_evolve): %s"), *StringValue);
}

void UEvolutionarySimulationComponent::InitializeRandomVectors()
{
    TemporalWeights.SetNum(PatternSize * TemporalSize);
    TimeWeights.SetNum(TemporalSize);
    BioMagicPatterns.SetNum(PatternSize);

    for (int32 i = 0; i < TemporalWeights.Num(); ++i)
    {
        TemporalWeights[i] = FMath::FRandRange(-1.0f,1.0f);
    }
    for (int32 i = 0; i < TimeWeights.Num(); ++i)
    {
        TimeWeights[i] = FMath::FRandRange(-1.0f,1.0f);
    }
    for (int32 i = 0; i < BioMagicPatterns.Num(); ++i)
    {
        BioMagicPatterns[i] = FMath::FRandRange(-1.0f,1.0f);
    }
}

TArray<double> UEvolutionarySimulationComponent::ComputeEvolutionaryPotential() const
{
    TArray<double> TemporalProjection;
    TemporalProjection.SetNum(TemporalSize);
    for (int32 Row = 0; Row < TemporalSize; ++Row)
    {
        double Sum = 0.0;
        for (int32 Col = 0; Col < PatternSize; ++Col)
        {
            Sum += TemporalWeights[Row * PatternSize + Col] * TimeWeights[Row];
        }
        TemporalProjection[Row] = Sum;
    }

    double DotTotal = 0.0;
    for (int32 Index = 0; Index < TemporalSize; ++Index)
    {
        DotTotal += TemporalProjection[Index] * BioMagicPatterns[Index % PatternSize];
    }

    TArray<double> Result;
    Result.Init(DotTotal, TemporalProjection.Num());
    return Softmax(Result);
}

TArray<double> UEvolutionarySimulationComponent::Softmax(const TArray<double>& Vector) const
{
    double MaxValue = Vector.Num() ? Vector[0] : 0.0;
    for (double Value : Vector)
    {
        MaxValue = FMath::Max(MaxValue, Value);
    }

    TArray<double> ExpValues;
    ExpValues.SetNum(Vector.Num());
    double SumExp = 0.0;
    for (int32 Index = 0; Index < Vector.Num(); ++Index)
    {
        double ExpValue = FMath::Exp(Vector[Index] - MaxValue);
        ExpValues[Index] = ExpValue;
        SumExp += ExpValue;
    }

    if (SumExp == 0.0)
    {
        return ExpValues;
    }

    for (int32 Index = 0; Index < ExpValues.Num(); ++Index)
    {
        ExpValues[Index] /= SumExp;
    }

    return ExpValues;
}
