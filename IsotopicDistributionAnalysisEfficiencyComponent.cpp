#include "IsotopicDistributionAnalysisEfficiencyComponent.h"
#include "Engine/Engine.h"

UIsotopicDistributionAnalysisEfficiencyComponent::UIsotopicDistributionAnalysisEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UIsotopicDistributionAnalysisEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateIDAE();
}

float UIsotopicDistributionAnalysisEfficiencyComponent::CalculateIDAE() const
{
	if (FMath::IsNearlyZero(DeltaTime))
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Time interval (Delta T) cannot be zero!"));
		return 0.0f;
	}

	return Lambda * (DeltaIPred / DeltaTime);
}

void UIsotopicDistributionAnalysisEfficiencyComponent::RecalculateIDAE()
{
	IDAE = CalculateIDAE();
	if (!FMath::IsNearlyZero(IDAE))
	{
		UE_LOG(LogTemp, Log, TEXT("Isotopic Distribution Analysis Efficiency (IDAE): %.4f"), IDAE);
	}
}
