#include "FantasyNeuralExpansionIndexComponent.h"
#include "Math/UnrealMathUtility.h"

UFantasyNeuralExpansionIndexComponent::UFantasyNeuralExpansionIndexComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Zeta = 1.0f;
	NFantasy = 0.0f;
	NTotal = 1.0f;
	FNEIValue = 0.0f;
}

void UFantasyNeuralExpansionIndexComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (NTotal <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Total number of neurons must be greater than zero."));
		FNEIValue = 0.0f;
	}
	else
	{
		FNEIValue = CalculateFNEI();
	}

	UE_LOG(LogTemp, Log, TEXT("FNEI: %.4f"), FNEIValue);
}

float UFantasyNeuralExpansionIndexComponent::CalculateFNEI() const
{
	return Zeta * FMath::Log2(1.0f + (NFantasy / NTotal));
}
