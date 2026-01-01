#include "NanoparticleBehaviorComponent.h"

UNanoparticleBehaviorComponent::UNanoparticleBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNanoparticleBehaviorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Velocity *= (1.0f - Drag * DeltaTime);

	if (AActor* Owner = GetOwner())
	{
		Owner->AddActorWorldOffset(Velocity * DeltaTime, true);
	}
}

void UNanoparticleBehaviorComponent::ApplyForce(const FVector& Force)
{
	Velocity += Force;
}
