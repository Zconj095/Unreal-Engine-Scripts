#include "EnergyTransformationEfficiencyComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyTransformationEfficiencyComponent::UEnergyTransformationEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	UsefulWork = 0.0f;
	TotalEnergy = 1.0f;
	Efficiency = 0.0f;
}

void UEnergyTransformationEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnergyTransformationEfficiencyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Efficiency = CalculateEfficiency(UsefulWork, TotalEnergy);
	LogStatus();
}

float UEnergyTransformationEfficiencyComponent::CalculateEfficiency(float Work, float Energy) const
{
	if (Energy <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Total energy absorbed (Q) must be greater than zero to calculate efficiency."));
		return 0.0f;
	}

	return Work / Energy;
}

void UEnergyTransformationEfficiencyComponent::LogStatus() const
{
	UE_LOG(LogTemp, Log, TEXT("Useful Work (W): %.2f"), UsefulWork);
	UE_LOG(LogTemp, Log, TEXT("Total Energy Absorbed (Q): %.2f"), TotalEnergy);
	UE_LOG(LogTemp, Log, TEXT("Energy Transformation Efficiency (η_t): %.2f"), Efficiency);
}
