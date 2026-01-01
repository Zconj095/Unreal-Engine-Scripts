#include "NanoParticleV2Component.h"
#include "Components/PrimitiveComponent.h"

UNanoParticleV2Component::UNanoParticleV2Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNanoParticleV2Component::BeginPlay()
{
	Super::BeginPlay();

	if (UMeshComponent* Primitive = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UMeshComponent::StaticClass())))
	{
		if (Primitive->GetMaterial(0))
		{
			Primitive->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), FVector(ParticleColor.R, ParticleColor.G, ParticleColor.B));
		}
	}
}

void UNanoParticleV2Component::SetParticleColor(const FLinearColor& Color)
{
	ParticleColor = Color;
	if (UMeshComponent* Primitive = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UMeshComponent::StaticClass())))
	{
		if (Primitive->GetMaterial(0))
		{
			Primitive->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), FVector(ParticleColor.R, ParticleColor.G, ParticleColor.B));
		}
	}
}

FVector2D UNanoParticleV2Component::InteractWithElectron(const FVector& ImpactDirection) const
{
	const float Absorbed = FMath::Clamp(InteractionParams.AbsorptionCoefficient * InteractionParams.MaterialDensity, 0.0f, 1.0f);
	const float Conducted = FMath::Clamp(InteractionParams.ConductionEfficiency * (1.0f - Absorbed), 0.0f, 1.0f);
	return FVector2D(Conducted, Absorbed);
}
