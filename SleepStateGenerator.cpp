#include "SleepStateGenerator.h"

#include "Components/SceneComponent.h"

ASleepStateGenerator::ASleepStateGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

void ASleepStateGenerator::BeginPlay()
{
    Super::BeginPlay();

    // Initialize array size similar to Unity Start()
    SleepStateComponents.SetNum(SleepStates.Num());
}

void ASleepStateGenerator::GenerateSleepStates()
{
    ClearExisting();

    const int32 Count = SleepStates.Num();
    SleepStateComponents.SetNum(Count);

    for (int32 i = 0; i < Count; ++i)
    {
        CreateSleepState(SleepStates[i], i);
    }
}

void ASleepStateGenerator::ClearExisting()
{
    for (USceneComponent* Comp : SleepStateComponents)
    {
        if (Comp)
        {
            Comp->DestroyComponent();
        }
    }
    SleepStateComponents.Empty();
}

void ASleepStateGenerator::CreateSleepState(const FString& StateName, int32 Index)
{
    USceneComponent* Comp = NewObject<USceneComponent>(this, *StateName);
    if (!Comp)
    {
        return;
    }

    Comp->SetupAttachment(Root);
    Comp->RegisterComponent();

    // Map Unity (index*2, -2, 0) to Unreal (X, Z, Y): X=Index*Spacing, Z=ZOffset, Y=0
    Comp->SetRelativeLocation(FVector(Index * SpacingCm, 0.f, ZOffsetCm));

    if (!SleepStateComponents.IsValidIndex(Index))
    {
        SleepStateComponents.SetNum(Index + 1);
    }
    SleepStateComponents[Index] = Comp;
}

