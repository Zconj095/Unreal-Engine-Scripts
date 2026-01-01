#include "LunarCycleManagerComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "GameFramework/WorldSettings.h"

ULunarCycleManagerComponent::ULunarCycleManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Default tags approximating Gaia layer names from Unity example
    AffectedTags = { FName("PW_Object_Small"), FName("PW_Object_Medium"), FName("PW_Object_Large") };
}

void ULunarCycleManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize immediately similar to Unity Start()
    UpdateGravityAndWater();
    UpdateCullingSettings();
}

void ULunarCycleManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Restore original cull distances
    for (auto& Pair : OriginalCullDistances)
    {
        if (UPrimitiveComponent* Comp = Pair.Key.Get())
        {
            const float* Original = OriginalCullDistances.Find(Pair.Key);
            if (Original)
            {
                // UE5.6: UPrimitiveComponent no longer exposes SetMaxDrawDistance; use LDMaxDrawDistance (editor-only data)
                Comp->LDMaxDrawDistance = *Original;
            }
        }
    }
    OriginalCullDistances.Empty();

    Super::EndPlay(EndPlayReason);
}

void ULunarCycleManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float LunarTimeNow = LunarTime;

    if (!FMath::IsNearlyEqual(LunarTimeNow, LastLunarTime))
    {
        LastLunarTime = LunarTimeNow;
        UpdateGravityAndWater();
    }

    if (bRealtimeUpdate)
    {
        UpdateCullingSettings();
    }
}

void ULunarCycleManagerComponent::UpdateGravityAndWater()
{
    ELunarPhase Phase = GetCurrentPhase(LunarTime);

    switch (Phase)
    {
    case ELunarPhase::NewMoon:
        CurrentGravityZ = BaseGravityZ;
        CurrentWaterLevelZ = BaseWaterLevelZ;
        break;
    case ELunarPhase::FirstQuarter:
        CurrentGravityZ = BaseGravityZ * (1.0f + GravityChangeFactor * 0.25f);
        CurrentWaterLevelZ = BaseWaterLevelZ + FullMoonWaterLevelChange * 0.25f;
        break;
    case ELunarPhase::FullMoon:
        CurrentGravityZ = BaseGravityZ * (1.0f + GravityChangeFactor);
        CurrentWaterLevelZ = BaseWaterLevelZ + FullMoonWaterLevelChange;
        break;
    case ELunarPhase::LastQuarter:
        CurrentGravityZ = BaseGravityZ * (1.0f + GravityChangeFactor * 0.25f);
        CurrentWaterLevelZ = BaseWaterLevelZ + FullMoonWaterLevelChange * 0.25f;
        break;
    }

    if (UWorld* World = GetWorld())
    {
        if (AWorldSettings* WS = World->GetWorldSettings())
        {
            WS->bGlobalGravitySet = true;
            WS->GlobalGravityZ = CurrentGravityZ; // Unreal uses cm/s^2; negative is downward
        }
    }

    ApplyWaterLevel();
}

void ULunarCycleManagerComponent::UpdateCullingSettings()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    const float PhaseFactor = GetLunarPhaseFactor(); // 0..1
    const float Scale = 1.0f + PhaseFactor * 0.25f; // Match Unity logic

    // Iterate all primitive components with any of the AffectedTags
    for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
    {
        UPrimitiveComponent* Comp = *It;
        if (!Comp || Comp->GetWorld() != World)
        {
            continue;
        }

        bool bHasTag = false;
        for (const FName& Tag : AffectedTags)
        {
            if (Comp->ComponentHasTag(Tag) || (Comp->GetOwner() && Comp->GetOwner()->ActorHasTag(Tag)))
            {
                bHasTag = true;
                break;
            }
        }
        if (!bHasTag)
        {
            continue;
        }

        // Cache original value once
        if (!OriginalCullDistances.Contains(Comp))
        {
            OriginalCullDistances.Add(Comp, Comp->LDMaxDrawDistance);
        }

        const float* Original = OriginalCullDistances.Find(Comp);
        if (Original)
        {
            const float NewDist = (*Original) * Scale + AdditionalCullingDistance;
            Comp->LDMaxDrawDistance = FMath::Max(0.0f, NewDist);
        }
    }
}

float ULunarCycleManagerComponent::GetLunarPhaseFactor() const
{
    // Returns 0 (new) .. 1 (full)
    const float LT = LunarTime;
    if (LT < 0.25f || LT >= 0.75f)
    {
        return 0.0f;
    }
    else if (LT >= 0.45f && LT < 0.55f)
    {
        return 1.0f;
    }
    else if (LT < 0.5f)
    {
        return (LT - 0.25f) * 4.0f;
    }
    else
    {
        return (0.75f - LT) * 4.0f;
    }
}

ELunarPhase ULunarCycleManagerComponent::GetCurrentPhase(float InLunarTime) const
{
    if (InLunarTime < 0.25f)
    {
        return ELunarPhase::NewMoon;
    }
    else if (InLunarTime < 0.5f)
    {
        return ELunarPhase::FirstQuarter;
    }
    else if (InLunarTime < 0.75f)
    {
        return ELunarPhase::FullMoon;
    }
    else
    {
        return ELunarPhase::LastQuarter;
    }
}

void ULunarCycleManagerComponent::ApplyWaterLevel()
{
    if (AActor* Water = ResolveWaterActor())
    {
        FVector P = Water->GetActorLocation();
        P.Z = CurrentWaterLevelZ;
        Water->SetActorLocation(P);
    }
}

AActor* ULunarCycleManagerComponent::ResolveWaterActor() const
{
    if (WaterActor.IsValid())
    {
        return WaterActor.Get();
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // Try tag on actors first
    if (!WaterActorTag.IsNone())
    {
        for (TActorIterator<AActor> It(World); It; ++It)
        {
            if (It->ActorHasTag(WaterActorTag))
            {
                return *It;
            }
        }
    }

    // Fallback: name match
    if (!WaterActorName.IsNone())
    {
        for (TActorIterator<AActor> It(World); It; ++It)
        {
            if (It->GetFName() == WaterActorName)
            {
                return *It;
            }
        }
    }

    return nullptr;
}
