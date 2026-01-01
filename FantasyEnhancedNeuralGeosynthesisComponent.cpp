#include "FantasyEnhancedNeuralGeosynthesisComponent.h"
#include "Math/UnrealMathUtility.h"

UFantasyEnhancedNeuralGeosynthesisComponent::UFantasyEnhancedNeuralGeosynthesisComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Zeta = 1.0f;
	NGeoFantasy = 0.0f;
	NTotal = 1.0f;
	FNGSValue = 0.0f;
}

void UFantasyEnhancedNeuralGeosynthesisComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFantasyEnhancedNeuralGeosynthesisComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (NTotal <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Total neurons (N_total) must be greater than zero."));
		FNGSValue = 0.0f;
		return;
	}

	FNGSValue = CalculateFNGS();
	UE_LOG(LogTemp, Log, TEXT("FNGS: %.4f"), FNGSValue);
}

float UFantasyEnhancedNeuralGeosynthesisComponent::CalculateFNGS() const
{
	return Zeta * FMath::Log2(1.0f + (NGeoFantasy / NTotal));
}
