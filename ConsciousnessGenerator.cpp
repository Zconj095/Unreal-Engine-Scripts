#include "ConsciousnessGenerator.h"

#include "Components/SceneComponent.h"

AConsciousnessGenerator::AConsciousnessGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

void AConsciousnessGenerator::BeginPlay()
{
    Super::BeginPlay();

    // Initialize array to match states length
    ConsciousnessStateComponents.SetNum(ConsciousnessStates.Num());
}

void AConsciousnessGenerator::GenerateConsciousnessStates()
{
    ClearExisting();

    const int32 Count = ConsciousnessStates.Num();
    ConsciousnessStateComponents.SetNum(Count);

    for (int32 i = 0; i < Count; ++i)
    {
        CreateConsciousnessState(ConsciousnessStates[i], i);
    }
}

void AConsciousnessGenerator::ClearExisting()
{
    for (USceneComponent* Comp : ConsciousnessStateComponents)
    {
        if (Comp)
        {
            Comp->DestroyComponent();
        }
    }
    ConsciousnessStateComponents.Empty();
}

void AConsciousnessGenerator::CreateConsciousnessState(const FString& StateName, int32 Index)
{
    USceneComponent* Comp = NewObject<USceneComponent>(this, *FString::Printf(TEXT("%s"), *StateName));
    if (!Comp)
    {
        return;
    }

    Comp->SetupAttachment(Root);
    Comp->RegisterComponent();

    // Place along X like Unity (index * 2.0f); using cm so multiply by SpacingCm
    Comp->SetRelativeLocation(FVector(Index * SpacingCm, 0.f, 0.f));

    if (!ConsciousnessStateComponents.IsValidIndex(Index))
    {
        ConsciousnessStateComponents.SetNum(Index + 1);
    }
    ConsciousnessStateComponents[Index] = Comp;
}

