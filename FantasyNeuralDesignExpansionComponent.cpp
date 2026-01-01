#include "FantasyNeuralDesignExpansionComponent.h"
#include "Math/UnrealMathUtility.h"

UFantasyNeuralDesignExpansionComponent::UFantasyNeuralDesignExpansionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Zeta = 1.0f;
	NFantasy = 0.0f;
	NTotal = 1.0f;
	Value = 0.0f;
}

void UFantasyNeuralDesignExpansionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (NTotal <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Total neurons (N_total) must be greater than zero."));
		Value = 0.0f;
	}
	else
	{
		Value = CalculateFNDE();
	}

	UE_LOG(LogTemp, Log, TEXT("FNDE: %.4f"), Value);
}

float UFantasyNeuralDesignExpansionComponent::CalculateFNDE() const
{
	return Zeta * FMath::Log2(1.0f + (NFantasy / NTotal));
}
