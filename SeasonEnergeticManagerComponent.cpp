#include "SeasonEnergeticManagerComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

USeasonEnergeticManagerComponent::USeasonEnergeticManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // Unity script applies once at Start
}

void USeasonEnergeticManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default presets if none have been authored
    if (ChakraSeasonTuning.Num() == 0 || AuraSeasonTuning.Num() == 0)
    {
        InitializeDefaultSeasonalTuning();
    }

    // Auto-resolve FX components if requested
    if (bAutoFindComponents)
    {
        if (!ChakraFX)
        {
            ChakraFX = AutoFindFX(ChakraComponentTag);
        }
        if (!AuraFX)
        {
            AuraFX = AutoFindFX(AuraComponentTag);
        }
    }

    ApplyChakraParticleBehavior();
    ApplyAuraParticleBehavior();
}

void USeasonEnergeticManagerComponent::InitializeDefaultSeasonalTuning()
{
    // Chakra defaults (match Unity values)
    ChakraSeasonTuning.Add(ESeason::Spring, {1.2f, 1.1f, 5.0f, 30.0f});
    ChakraSeasonTuning.Add(ESeason::Summer, {1.5f, 1.3f, 4.5f, 40.0f});
    ChakraSeasonTuning.Add(ESeason::Autumn, {1.0f, 1.0f, 6.0f, 25.0f});
    ChakraSeasonTuning.Add(ESeason::Winter, {0.7f, 1.2f, 7.0f, 20.0f});

    // Aura defaults (match Unity values)
    AuraSeasonTuning.Add(ESeason::Spring, {1.3f, 1.2f, 5.0f, 35.0f});
    AuraSeasonTuning.Add(ESeason::Summer, {1.7f, 1.5f, 4.0f, 50.0f});
    AuraSeasonTuning.Add(ESeason::Autumn, {1.0f, 1.0f, 6.5f, 30.0f});
    AuraSeasonTuning.Add(ESeason::Winter, {0.6f, 1.2f, 7.5f, 15.0f});
}

void USeasonEnergeticManagerComponent::ApplyChakraParticleBehavior()
{
    const FSeasonParticleTuning* Tuning = ChakraSeasonTuning.Find(Season);
    if (ChakraFX && Tuning)
    {
        ApplyTuningToFX(ChakraFX, *Tuning, ParamName_Speed, ParamName_Size, ParamName_Lifetime, ParamName_EmissionRate);
        UE_LOG(LogTemp, Log, TEXT("Applied Chakra Particle Behavior for %d"), static_cast<int32>(Season));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Chakra FX or tuning missing for season %d"), static_cast<int32>(Season));
    }
}

void USeasonEnergeticManagerComponent::ApplyAuraParticleBehavior()
{
    const FSeasonParticleTuning* Tuning = AuraSeasonTuning.Find(Season);
    if (AuraFX && Tuning)
    {
        ApplyTuningToFX(AuraFX, *Tuning, ParamName_Speed, ParamName_Size, ParamName_Lifetime, ParamName_EmissionRate);
        UE_LOG(LogTemp, Log, TEXT("Applied Aura Particle Behavior for %d"), static_cast<int32>(Season));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Aura FX or tuning missing for season %d"), static_cast<int32>(Season));
    }
}

/* static */
void USeasonEnergeticManagerComponent::ApplyTuningToFX(UFXSystemComponent* FX, const FSeasonParticleTuning& Tuning,
                                                       const FName& P_Speed, const FName& P_Size,
                                                       const FName& P_Lifetime, const FName& P_EmissionRate)
{
    if (!FX)
    {
        return;
    }

    // Call FX parameter setters via reflection to avoid engine header dependency
    auto SetFXFloat = [](UObject* Obj, const FName& Param, float Value)
    {
        if (!Obj || Param.IsNone()) return;
        static const FName FuncName_SetFloat("SetFloatParameter");
        if (UFunction* Fn = Obj->FindFunction(FuncName_SetFloat))
        {
            struct { FName Name; float Val; } Params{ Param, Value };
            Obj->ProcessEvent(Fn, &Params);
        }
    };

    SetFXFloat(reinterpret_cast<UObject*>(FX), P_Speed, Tuning.Speed);
    SetFXFloat(reinterpret_cast<UObject*>(FX), P_Size, Tuning.Size);
    SetFXFloat(reinterpret_cast<UObject*>(FX), P_Lifetime, Tuning.Lifetime);
    SetFXFloat(reinterpret_cast<UObject*>(FX), P_EmissionRate, Tuning.EmissionRate);
}

UFXSystemComponent* USeasonEnergeticManagerComponent::AutoFindFX(const FName& DesiredTagOrName) const
{
    if (AActor* Owner = GetOwner())
    {
        TArray<UActorComponent*> Comps;
        Owner->GetComponents(Comps);
        for (UActorComponent* C : Comps)
        {
            if (!C) continue;
            const bool bTagMatch = DesiredTagOrName.IsNone() ? false : C->ComponentHasTag(DesiredTagOrName);
            const bool bNameMatch = (C->GetFName() == DesiredTagOrName);
            if (bTagMatch || bNameMatch)
            {
                return reinterpret_cast<UFXSystemComponent*>(C);
            }
        }
    }
    return nullptr;
}
