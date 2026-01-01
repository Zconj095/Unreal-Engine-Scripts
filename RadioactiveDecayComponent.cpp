#include "RadioactiveDecayComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

URadioactiveDecayComponent::URadioactiveDecayComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URadioactiveDecayComponent::BeginPlay()
{
	Super::BeginPlay();
	Quantity = CalculateRadioactiveDecay();
	UE_LOG(LogTemp, Log, TEXT("Radioactive Quantity at Time %0.2fs: %0.4f"), TimeSeconds, Quantity);
}

float URadioactiveDecayComponent::CalculateRadioactiveDecay() const
{
	return InitialQuantity * FMath::Exp(-DecayConstant * TimeSeconds) + MagicalEnergyFactor;
}
