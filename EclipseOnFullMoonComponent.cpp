#include "EclipseOnFullMoonComponent.h"

#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"

UEclipseOnFullMoonComponent::UEclipseOnFullMoonComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UEclipseOnFullMoonComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!TargetDirectionalLight && bAutoFindMoonLight)
    {
        TryAutoFindMoonLight();
    }

    if (UDirectionalLightComponent* LightComp = GetTargetLightComponent())
    {
        OriginalIntensity = LightComp->Intensity;
    }
}

void UEclipseOnFullMoonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsFullMoon())
    {
        TriggerEclipse();
    }
    else
    {
        ResetEclipse();
    }
}

bool UEclipseOnFullMoonComponent::IsFullMoon() const
{
    // Consider both near 0 and near 1 as full moon (wrapping continuity)
    return FMath::Abs(CurrentMoonPhase - 0.0f) <= FullMoonThreshold ||
           FMath::Abs(CurrentMoonPhase - 1.0f) <= FullMoonThreshold;
}

UDirectionalLightComponent* UEclipseOnFullMoonComponent::GetTargetLightComponent() const
{
    if (!TargetDirectionalLight)
    {
        return nullptr;
    }

    return Cast<UDirectionalLightComponent>(TargetDirectionalLight->GetLightComponent());
}

void UEclipseOnFullMoonComponent::TryAutoFindMoonLight()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    ADirectionalLight* Fallback = nullptr;

    for (TActorIterator<ADirectionalLight> It(World); It; ++It)
    {
        ADirectionalLight* Dir = *It;
        if (!Dir)
        {
            continue;
        }

        if (!Fallback)
        {
            Fallback = Dir;
        }

        if (UDirectionalLightComponent* Comp = Cast<UDirectionalLightComponent>(Dir->GetLightComponent()))
        {
            // Common convention: AtmosphereSunLightIndex 0 = Sun, 1 = Moon
            if (Comp->AtmosphereSunLightIndex == 1)
            {
                TargetDirectionalLight = Dir;
                return;
            }
        }
    }

    // If no explicit moon light was found, use the first directional light as a fallback.
    TargetDirectionalLight = Fallback;
}

void UEclipseOnFullMoonComponent::TriggerEclipse()
{
    if (UDirectionalLightComponent* LightComp = GetTargetLightComponent())
    {
        if (OriginalIntensity < 0.0f)
        {
            OriginalIntensity = LightComp->Intensity;
        }

        const float NewIntensity = OriginalIntensity * EclipseIntensityMultiplier;

        // Only apply if not already applied (within tolerance)
        if (!bEclipseApplied || !FMath::IsNearlyEqual(LightComp->Intensity, NewIntensity, 0.1f))
        {
            LightComp->SetIntensity(NewIntensity);
            bEclipseApplied = true;
            UE_LOG(LogTemp, Log, TEXT("Eclipse triggered on full moon. Intensity set to %f"), NewIntensity);
        }
    }
}

void UEclipseOnFullMoonComponent::ResetEclipse()
{
    if (!bEclipseApplied)
    {
        return;
    }

    if (UDirectionalLightComponent* LightComp = GetTargetLightComponent())
    {
        if (OriginalIntensity >= 0.0f)
        {
            LightComp->SetIntensity(OriginalIntensity);
        }
    }

    bEclipseApplied = false;
}

