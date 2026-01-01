#include "EnergyTransferModelComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyTransferModelComponent::UEnergyTransferModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MagicalEnhancement = 2.0;
	DeltaHs = { -50.0, -40.0, -30.0 };
	Temperatures = { 300.0, 310.0, 320.0 };
	DeltaSs = { 0.1, 0.2, 0.3 };
	MagicalEnergies = { 10.0, 15.0, 20.0 };
}

void UEnergyTransferModelComponent::BeginPlay()
{
	Super::BeginPlay();

	SimulateEnergyChanges();
	for (int32 Index = 0; Index < EnergyChanges.Num(); ++Index)
	{
		UE_LOG(LogTemp, Log, TEXT("Condition %d: ΔG = %.4f"), Index + 1, EnergyChanges[Index]);
	}
}

double UEnergyTransferModelComponent::ComputeFreeEnergy(double DeltaH, double Temperature, double DeltaS, double MagicalEnergy) const
{
	return DeltaH - (Temperature * DeltaS) - (MagicalEnhancement * MagicalEnergy);
}

void UEnergyTransferModelComponent::SimulateEnergyChanges()
{
	if (DeltaHs.Num() != Temperatures.Num() || Temperatures.Num() != DeltaSs.Num() || DeltaSs.Num() != MagicalEnergies.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Input arrays must match in length."));
		return;
	}

	EnergyChanges.Reset();
	EnergyChanges.SetNum(DeltaHs.Num());

	for (int32 Index = 0; Index < DeltaHs.Num(); ++Index)
	{
		EnergyChanges[Index] = ComputeFreeEnergy(DeltaHs[Index], Temperatures[Index], DeltaSs[Index], MagicalEnergies[Index]);
	}
}
