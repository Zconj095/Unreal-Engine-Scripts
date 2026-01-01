#include "DataInitialize1Component.h"

#include "Math/UnrealMathUtility.h"
#include "NeuralEncodingProcessorLibrary.h"

UDataInitialize1Component::UDataInitialize1Component()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDataInitialize1Component::BeginPlay()
{
    Super::BeginPlay();

    // Example data initialization
    TArray<float> Stimuli1 = GenerateRandomData(1000);
    TArray<float> Stimuli2 = GenerateRandomData(1000);

    // Processing data
    TArray<float> Differences;
    UNeuralEncodingProcessorLibrary::DifferenceBasedCoding(Stimuli1, Stimuli2, Differences);

    TArray<float> StatisticallyEncoded;
    UNeuralEncodingProcessorLibrary::StatisticalEncoding(Differences, StatisticallyEncoded);

    TArray<float> RelationallyDetermined;
    UNeuralEncodingProcessorLibrary::RelationallyDeterminedProcess(Differences, RelationallyDetermined);

    TArray<float> Stimulus = GenerateRandomData(1000);
    TArray<float> PhysicallyEncoded;
    UNeuralEncodingProcessorLibrary::PhysicallyBasedEncoding(Stimulus, PhysicallyEncoded);

    // Logging the output
    UE_LOG(LogTemp, Log, TEXT("Statistically encoded values: %s"), *ArrayToString(StatisticallyEncoded));
    UE_LOG(LogTemp, Log, TEXT("Physically encoded values: %s"), *ArrayToString(PhysicallyEncoded));
    UE_LOG(LogTemp, Log, TEXT("Relationally determined values: %s"), *ArrayToString(RelationallyDetermined));
}

TArray<float> UDataInitialize1Component::GenerateRandomData(int32 Length) const
{
    TArray<float> Data; Data.SetNum(FMath::Max(0, Length));
    for (int32 i = 0; i < Data.Num(); ++i)
    {
        Data[i] = FMath::FRand();
    }
    return Data;
}

FString UDataInitialize1Component::ArrayToString(const TArray<float>& Array) const
{
    FString S = TEXT("[");
    for (int32 i = 0; i < Array.Num(); ++i)
    {
        if (i > 0) { S += TEXT(", "); }
        S += FString::SanitizeFloat(Array[i]);
    }
    S += TEXT("]");
    return S;
}

