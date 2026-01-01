#include "EnergyFlowModelComponent.h"

UEnergyFlowModelComponent::UEnergyFlowModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	EnergyContributions = { 100.0, 80.0, 50.0 };
	TrophicEfficiencies = { 0.8, 0.5, 0.2 };
	TotalEnergyFlow = 0.0;
}

void UEnergyFlowModelComponent::BeginPlay()
{
	Super::BeginPlay();

	TotalEnergyFlow = ComputeTotalEnergyFlow(EnergyContributions, TrophicEfficiencies);
	UE_LOG(LogTemp, Log, TEXT("Total Energy Flow in Ecosystem: %.4f"), TotalEnergyFlow);
	LogExample();
}

double UEnergyFlowModelComponent::ComputeTotalEnergyFlow(const TArray<double>& Contributions, const TArray<double>& Efficiency) const
{
	if (Contributions.Num() != Efficiency.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Energy contributions and trophic efficiencies must be equal length."));
		return 0.0;
	}

	double TotalEnergy = 0.0;
	for (int32 Index = 0; Index < Contributions.Num(); ++Index)
	{
		TotalEnergy += Contributions[Index] * Efficiency[Index];
	}
	return TotalEnergy;
}

void UEnergyFlowModelComponent::LogExample() const
{
	UE_LOG(LogTemp, Log, TEXT("Example energy contributions and efficiencies provided through editable arrays."));
}
