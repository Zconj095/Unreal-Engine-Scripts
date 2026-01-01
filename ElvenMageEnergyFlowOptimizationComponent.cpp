#include "ElvenMageEnergyFlowOptimizationComponent.h"
#include "Math/UnrealMathUtility.h"

UElvenMageEnergyFlowOptimizationComponent::UElvenMageEnergyFlowOptimizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	HeatTransfer = 500.0;
	WorkDone = 200.0;
	MagicalContribution = 100.0;
	Resistances = { 1.0, 2.0, 3.0 };
}

void UElvenMageEnergyFlowOptimizationComponent::BeginPlay()
{
	Super::BeginPlay();

	LogEnergyChange();

	const double TotalEnergy = CalculateEnergyChange(HeatTransfer, WorkDone, MagicalContribution);
	const double Power = CalculatePower(TotalEnergy, Resistances);
	UE_LOG(LogTemp, Log, TEXT("Calculated Power: %f"), Power);
}

void UElvenMageEnergyFlowOptimizationComponent::LogEnergyChange() const
{
	const double EnergyChange = CalculateEnergyChange(HeatTransfer, WorkDone, MagicalContribution);
	UE_LOG(LogTemp, Log, TEXT("Change in Energy: %f J"), EnergyChange);
}

double UElvenMageEnergyFlowOptimizationComponent::CalculateEnergyChange(double Heat, double Work, double Magic) const
{
	return Heat - Work + Magic;
}

double UElvenMageEnergyFlowOptimizationComponent::CalculatePower(double TotalEnergy, const TArray<double>& ResistanceArray) const
{
	double TotalResistance = 0.0;
	for (double Resistance : ResistanceArray)
	{
		TotalResistance += Resistance;
	}

	if (FMath::IsNearlyZero(TotalResistance))
	{
		UE_LOG(LogTemp, Warning, TEXT("Total resistance cannot be zero."));
		return 0.0;
	}

	return TotalEnergy / TotalResistance;
}
