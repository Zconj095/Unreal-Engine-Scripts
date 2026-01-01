#include "ExternalFieldController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

UExternalFieldController::UExternalFieldController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UExternalFieldController::BeginPlay()
{
	Super::BeginPlay();
}

void UExternalFieldController::ApplyField(AActor* TargetParticle) const
{
	if (!TargetParticle)
	{
		return;
	}

	const FVector NormalizedDirection = FieldDirection.IsNearlyZero() ? FVector::UpVector : FieldDirection.GetSafeNormal();
	const FVector Force = NormalizedDirection * FieldStrength;

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	TargetParticle->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	for (UPrimitiveComponent* Primitive : PrimitiveComponents)
	{
		if (Primitive && Primitive->IsSimulatingPhysics())
		{
			Primitive->AddForce(Force, NAME_None, true);
			return;
		}
	}

	if (AActor* Owner = GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("ExternalFieldController: No simulating primitive found on %s"), *TargetParticle->GetName());
	}
}
