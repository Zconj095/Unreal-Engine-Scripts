#include "CreativeNeuralSynthesisIndex.h"

DEFINE_LOG_CATEGORY(LogCreativeNeuralSynthesisIndex);

UCreativeNeuralSynthesisIndex::UCreativeNeuralSynthesisIndex()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCreativeNeuralSynthesisIndex::BeginPlay()
{
    Super::BeginPlay();
    RecalculateCNSI();
}

void UCreativeNeuralSynthesisIndex::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IdeaInputs.Num() != IdeaEfforts.Num())
    {
        UE_LOG(LogCreativeNeuralSynthesisIndex, Error, TEXT("Idea inputs and efforts must have the same length."));
        CNSIValue = 0.f;
        return;
    }

    if (CognitiveTime <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogCreativeNeuralSynthesisIndex, Error, TEXT("Cognitive time (T) must be greater than zero."));
        CNSIValue = 0.f;
        return;
    }

    RecalculateCNSI();
    UE_LOG(LogCreativeNeuralSynthesisIndex, Verbose, TEXT("CNSI: %.6f"), CNSIValue);
}

void UCreativeNeuralSynthesisIndex::UpdateInputs(const TArray<float>& NewInputs)
{
    IdeaInputs = NewInputs;
    RecalculateCNSI();
}

void UCreativeNeuralSynthesisIndex::UpdateEfforts(const TArray<float>& NewEfforts)
{
    IdeaEfforts = NewEfforts;
    RecalculateCNSI();
}

void UCreativeNeuralSynthesisIndex::RecalculateCNSI()
{
    if (IdeaInputs.Num() != IdeaEfforts.Num() || CognitiveTime <= KINDA_SMALL_NUMBER)
    {
        CNSIValue = 0.f;
        return;
    }

    float Sum = 0.f;
    for (int32 Index = 0; Index < IdeaInputs.Num(); ++Index)
    {
        Sum += IdeaInputs[Index] * IdeaEfforts[Index];
    }

    CNSIValue = Alpha * (Sum / CognitiveTime);
}
