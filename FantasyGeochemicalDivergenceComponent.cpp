#include "FantasyGeochemicalDivergenceComponent.h"
#include "Math/UnrealMathUtility.h"

UFantasyGeochemicalDivergenceComponent::UFantasyGeochemicalDivergenceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DeltaPsi = 3.5f;
	Omega = 2.0f;
	Rho = 1.2f;
	FGDRValue = 0.0f;
}

void UFantasyGeochemicalDivergenceComponent::BeginPlay()
{
	Super::BeginPlay();

	FGDRValue = CalculateFGDR(DeltaPsi, Omega, Rho);
	UE_LOG(LogTemp, Log, TEXT("Fantasy Geochemical Divergence Ratio (FGDR): %f"), FGDRValue);
}

float UFantasyGeochemicalDivergenceComponent::CalculateFGDR(float InDeltaPsi, float InOmega, float InRho) const
{
	if (FMath::IsNearlyZero(InOmega))
	{
		UE_LOG(LogTemp, Error, TEXT("Total cognitive energy (Omega) cannot be zero!"));
		return 0.0f;
	}

	return (InDeltaPsi / InOmega) * InRho;
}
