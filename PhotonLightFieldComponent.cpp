// Copyright Epic Games, Inc.

#include "PhotonLightFieldComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "Logging/LogMacros.h"

UPhotonLightFieldComponent::UPhotonLightFieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPhotonLightFieldComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeLightField();
}

void UPhotonLightFieldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AActor* Owner = GetOwner())
	{
		Owner->AddActorLocalRotation(FRotator(0.f, PrismRotationSpeed * DeltaTime, 0.f));
	}
}

void UPhotonLightFieldComponent::InitializeLightField()
{
	if (!ParticleSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhotonLightFieldComponent: ParticleSystem is not assigned."));
		return;
	}

	// Try to drive parameters; requires matching parameter names in the particle system asset.
	ParticleSystem->SetFloatParameter(TEXT("MaxParticles"), NumParticles);
	ParticleSystem->SetFloatParameter(TEXT("FieldRadius"), FieldRadius);
	ParticleSystem->SetFloatParameter(TEXT("LuminescenceIntensity"), LuminescenceIntensity);

	ParticleSystem->ActivateSystem(true);
}

void UPhotonLightFieldComponent::FormPrism()
{
	if (!ParticleSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhotonLightFieldComponent: ParticleSystem is not assigned."));
		return;
	}

	// Set parameters expected by your particle system to represent a prism/cone.
	ParticleSystem->SetFloatParameter(TEXT("PrismAngle"), 15.f);
	ParticleSystem->SetFloatParameter(TEXT("PrismBaseRadius"), PrismBaseRadius);
	ParticleSystem->SetFloatParameter(TEXT("PrismHeight"), PrismHeight);

	UE_LOG(LogTemp, Log, TEXT("Prism Formation Activated."));
}
