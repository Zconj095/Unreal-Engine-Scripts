#include "EnvironmentalEnergyAbsorptionComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

UEnvironmentalEnergyAbsorptionComponent::UEnvironmentalEnergyAbsorptionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	AbsorptionEfficiency = 0.5f;
	SimulationDuration = 10.0f;
	Resolution = 100;
}

void UEnvironmentalEnergyAbsorptionComponent::BeginPlay()
{
	Super::BeginPlay();

	GenerateEnvironmentalValues();
	const float AbsorbedEnergy = CalculateEnergyAbsorption();
	UE_LOG(LogTemp, Log, TEXT("Total Absorbed Energy (Ea): %f"), AbsorbedEnergy);

	if (GetWorld())
	{
		const float Dt = SimulationDuration / Resolution;
		for (int32 Index = 0; Index < EnvironmentalValues.Num() - 1; ++Index)
		{
			const float T1 = Index * Dt;
			const float T2 = (Index + 1) * Dt;
			const FVector P1(T1, EnvironmentalValues[Index], 0.0f);
			const FVector P2(T2, EnvironmentalValues[Index + 1], 0.0f);
			DrawDebugLine(GetWorld(), P1, P2, FColor::Green, false, 10.0f);
		}
	}
}

void UEnvironmentalEnergyAbsorptionComponent::GenerateEnvironmentalValues()
{
	EnvironmentalValues.SetNum(Resolution);
	const float Scale = 2 * PI / SimulationDuration;
	for (int32 Index = 0; Index < Resolution; ++Index)
	{
		const float Time = Index * (SimulationDuration / Resolution);
		EnvironmentalValues[Index] = FMath::Sin(Scale * Time) + 1.0f;
	}
}

float UEnvironmentalEnergyAbsorptionComponent::CalculateEnergyAbsorption() const
{
	if (EnvironmentalValues.Num() < 2 || Resolution <= 0)
	{
		return 0.0f;
	}

	const float Dt = SimulationDuration / Resolution;
	float EnergyAbsorption = 0.0f;
	for (int32 Index = 0; Index < EnvironmentalValues.Num() - 1; ++Index)
	{
		const float Ve1 = EnvironmentalValues[Index];
		const float Ve2 = EnvironmentalValues[Index + 1];
		EnergyAbsorption += 0.5f * (Ve1 + Ve2) * Dt;
	}
	return AbsorptionEfficiency * EnergyAbsorption;
}
