#include "DataInitializeComponent.h"

#include "Math/UnrealMathUtility.h"
#include "NeuralCodingProcessorLibrary.h"

UDataInitializeComponent::UDataInitializeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDataInitializeComponent::BeginPlay()
{
    Super::BeginPlay();

    // Example data initialization
    TArray<float> Stimuli1 = RandomValues(1000);
    TArray<float> Stimuli2 = RandomValues(1000);
    TArray<float> Differences;
    UNeuralCodingProcessorLibrary::DifferenceBasedCoding(Stimuli1, Stimuli2, Differences);

    TArray<float> Stimulus = RandomValues(1000);
    TArray<float> NeuralActivityValues;
    UNeuralCodingProcessorLibrary::NeuralActivity(Differences, Stimulus, NeuralActivityValues);

    for (float Activity : NeuralActivityValues)
    {
        UE_LOG(LogTemp, Log, TEXT("Neural Activity: %f"), Activity);
    }
}

TArray<float> UDataInitializeComponent::RandomValues(int32 Count) const
{
    TArray<float> Values; Values.SetNum(FMath::Max(0, Count));
    for (int32 i = 0; i < Values.Num(); ++i)
    {
        Values[i] = FMath::FRand();
    }
    return Values;
}

