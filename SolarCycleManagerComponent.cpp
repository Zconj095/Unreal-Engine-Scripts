#include "SolarCycleManagerComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/ExponentialHeightFog.h"

namespace
{
    constexpr double SecondsPerYear = 365.0 * 24.0 * 3600.0;
}

USolarCycleManagerComponent::USolarCycleManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USolarCycleManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    // Find a fog component in the world to control
    if (UWorld* World = GetWorld())
    {
        for (TActorIterator<AExponentialHeightFog> It(World); It; ++It)
        {
            if (UExponentialHeightFogComponent* C = It->GetComponent())
            {
                FogComp = C;
                OriginalFogDensity = C->FogDensity;
                // Initialize to base
                C->SetFogDensity(BaseAtmosphericFogDensity);
                break;
            }
        }
    }

    // Attempt to auto-find aurora FX if not set
    if (!AuroraFX)
    {
        AuroraFX = AutoFindFXOnOwner(AuroraFXTagOrName);
    }

    // One initial update akin to Unity Start
    UpdateSolarEffects();
}

void USolarCycleManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (FogComp.IsValid() && OriginalFogDensity >= 0.0f)
    {
        FogComp.Get()->SetFogDensity(OriginalFogDensity);
    }

    Super::EndPlay(EndPlayReason);
}

void USolarCycleManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Advance solar cycle if simulating full cycle
    if (bSimulateFullCycle && SolarCycleDurationYears > 0.0f)
    {
        const double Den = SecondsPerYear * SolarCycleDurationYears;
        const double Delta = (static_cast<double>(DeltaTime) * static_cast<double>(SimulationSpeed)) / Den;
        CurrentCycleProgress = FMath::Fmod(static_cast<float>(CurrentCycleProgress + Delta), 1.0f);
        if (CurrentCycleProgress < 0.0f)
        {
            CurrentCycleProgress += 1.0f;
        }
    }

    // Daytime
    float DayNorm = DayTimeNormalized;
    if (bAutoDayProgress && DayLengthSeconds > 0.0f)
    {
        const float T = GetWorld()->GetTimeSeconds();
        DayNorm = FMath::Fmod(T, DayLengthSeconds) / DayLengthSeconds;
    }

    // Update tide if day time changed
    if (!FMath::IsNearlyEqual(DayNorm, LastDayTimeNorm))
    {
        LastDayTimeNorm = DayNorm;
        UpdateSolarTide(DayNorm);
    }

    // Update environmental effects at ~1 Hz
    SecondAccumulator += DeltaTime;
    if (SecondAccumulator >= 1.0f)
    {
        SecondAccumulator = FMath::Fmod(SecondAccumulator, 1.0f);
        UpdateSolarEffects();
    }

    if (bDrawDebug)
    {
        const float Influence = GetSolarCycleInfluence();
        const float Radius = Influence * 200.0f; // arbitrary scale
        DrawDebugSphere(GetWorld(), GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector,
                        Radius, 16, FColor::Yellow, false, -1.0f, 0, 2.0f);
    }
}

float USolarCycleManagerComponent::GetSolarCycleInfluence() const
{
    // Max at 0.5, min at 0 or 1
    return 1.0f - FMath::Abs((CurrentCycleProgress - 0.5f) * 2.0f);
}

void USolarCycleManagerComponent::UpdateSolarEffects()
{
    const float Influence = GetSolarCycleInfluence();
    UpdateAtmosphericEffects(Influence);
    UpdateAuroraEffects(Influence);
    UpdateSolarWindEffects(Influence);
}

void USolarCycleManagerComponent::UpdateSolarTide(float DayNorm)
{
    // Daily tide factor: sin(day*2pi) mapped to 0..1
    const float DailyTideFactor = FMath::Sin(DayNorm * 2.0f * UE_PI) * 0.5f + 0.5f;
    const float CycleInfluence = GetSolarCycleInfluence();

    CurrentSolarTideHeightZ = BaseSolarTideHeightZ
        + (DailyTideFactor * SolarTideVariationZ)
        + (CycleInfluence * SolarMaximumTideBoostZ)
        + AdditionalWaterOffsetZ;

    UpdateCombinedTideEffects();
}

void USolarCycleManagerComponent::UpdateAtmosphericEffects(float CycleInfluence)
{
    const float CurrentDensity = BaseAtmosphericFogDensity * (1.0f + CycleInfluence * 0.1f);
    if (FogComp.IsValid())
    {
        FogComp.Get()->SetFogDensity(CurrentDensity);
    }
}

void USolarCycleManagerComponent::UpdateAuroraEffects(float CycleInfluence)
{
    if (!AuroraFX)
    {
        return;
    }

    const float CurrentIntensity = AuroraIntensity * CycleInfluence;
    const FLinearColor CurrentColor = FLinearColor::LerpUsingHSV(AuroraBaseColor, FLinearColor(0.8f, 0.2f, 0.2f), CycleInfluence);

    auto SetFXFloat = [](UObject* Obj, const FName& Param, float Value)
    {
        if (!Obj || Param.IsNone()) return;
        static const FName FuncName("SetFloatParameter");
        if (UFunction* Fn = Obj->FindFunction(FuncName))
        {
            struct { FName Name; float Val; } Params{ Param, Value };
            Obj->ProcessEvent(Fn, &Params);
        }
    };
    auto SetFXColor = [](UObject* Obj, const FName& Param, const FLinearColor& Value)
    {
        if (!Obj || Param.IsNone()) return;
        static const FName FuncName("SetColorParameter");
        if (UFunction* Fn = Obj->FindFunction(FuncName))
        {
            struct { FName Name; FLinearColor Val; } Params{ Param, Value };
            Obj->ProcessEvent(Fn, &Params);
        }
    };

    SetFXFloat(reinterpret_cast<UObject*>(AuroraFX), Param_AuroraIntensity, CurrentIntensity);
    SetFXColor(reinterpret_cast<UObject*>(AuroraFX), Param_AuroraColor, CurrentColor);
}

void USolarCycleManagerComponent::UpdateSolarWindEffects(float CycleInfluence)
{
    const float CurrentSolarWind = SolarWindStrength * (1.0f + CycleInfluence);

    auto SetFXFloat = [](UObject* Obj, const FName& Param, float Value)
    {
        if (!Obj || Param.IsNone()) return;
        static const FName FuncName("SetFloatParameter");
        if (UFunction* Fn = Obj->FindFunction(FuncName))
        {
            struct { FName Name; float Val; } Params{ Param, Value };
            Obj->ProcessEvent(Fn, &Params);
        }
    };

    if (SolarWindFX.Num() > 0)
    {
        for (UFXSystemComponent* FX : SolarWindFX)
        {
            if (FX)
            {
                SetFXFloat(reinterpret_cast<UObject*>(FX), Param_SolarWindSpeed, CurrentSolarWind);
            }
        }
    }
    else if (AActor* Owner = GetOwner())
    {
        TArray<UActorComponent*> Comps;
        Owner->GetComponents(Comps);
        for (UActorComponent* C : Comps)
        {
            SetFXFloat(C, Param_SolarWindSpeed, CurrentSolarWind);
        }
    }
}

void USolarCycleManagerComponent::UpdateCombinedTideEffects()
{
    if (!bWriteWaterHeight)
    {
        return;
    }

    if (AActor* Water = ResolveWaterActor())
    {
        FVector P = Water->GetActorLocation();
        P.Z = CurrentSolarTideHeightZ;
        Water->SetActorLocation(P);
    }
}

AActor* USolarCycleManagerComponent::ResolveWaterActor() const
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

UFXSystemComponent* USolarCycleManagerComponent::AutoFindFXOnOwner(const FName& TagOrName) const
{
    if (AActor* Owner = GetOwner())
    {
        TArray<UActorComponent*> Comps;
        Owner->GetComponents(Comps);
        for (UActorComponent* C : Comps)
        {
            if (!C) continue;
            const bool bTagMatch = TagOrName.IsNone() ? false : C->ComponentHasTag(TagOrName);
            const bool bNameMatch = (C->GetFName() == TagOrName);
            if (bTagMatch || bNameMatch)
            {
                return reinterpret_cast<UFXSystemComponent*>(C);
            }
        }
    }
    return nullptr;
}
