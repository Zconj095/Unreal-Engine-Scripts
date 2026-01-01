#include "NanoParticleComponent.h"
#include "Components/PrimitiveComponent.h"

UNanoParticleComponent::UNanoParticleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNanoParticleComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UMeshComponent* Primitive = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UMeshComponent::StaticClass())))
	{
		if (Primitive->GetMaterial(0))
		{
			Primitive->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), FVector(ParticleColor.R, ParticleColor.G, ParticleColor.B));
		}
		else
		{
			Primitive->SetRenderCustomDepth(true);
			Primitive->SetCustomDepthStencilValue(0);
		}
	}
}
