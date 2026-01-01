#include "NanoscopicStructureComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

UNanoscopicStructureComponent::UNanoscopicStructureComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UNanoscopicStructureComponent::CalculateField(const FVector& Point) const
{
	const FVector Origin = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
	const FVector Direction = Point - Origin;
	const float Distance = Direction.Size();

	if (Distance > InteractionRadius || Distance <= KINDA_SMALL_NUMBER)
	{
		return FVector::ZeroVector;
	}

	const float FieldStrength = FieldIntensity / FMath::Square(Distance);
	return Direction.GetSafeNormal() * FieldStrength * Charge;
}
