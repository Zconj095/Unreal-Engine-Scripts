#include "ReactionSimulationPotentialCalculatorComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UReactionSimulationPotentialCalculatorComponent::UReactionSimulationPotentialCalculatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UReactionSimulationPotentialCalculatorComponent::BeginPlay()
{
	Super::BeginPlay();

	Potential = CalculateReactionSimulationPotential();
	UE_LOG(LogTemp, Log, TEXT("Reaction Simulation Potential (P_r): %0.2f"), Potential);
}

float UReactionSimulationPotentialCalculatorComponent::CalculateReactionSimulationPotential() const
{
	if (Steps <= 0 || EndTime <= StartTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration parameters."));
		return 0.0f;
	}

	const float DX = (EndTime - StartTime) / Steps;
	float Sum = 0.0f;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float CurrentX = StartTime + Index * DX;
		const float NextX = CurrentX + DX;
		const float MidpointX = (CurrentX + NextX) * 0.5f;
		const float Psi = FMath::Sin(MidpointX + EndTime);
		const float HamiltonianApplied = HamiltonianFactor * Psi;
		const float Integrand = Psi * HamiltonianApplied * Psi;
		Sum += Integrand * DX;
	}

	return Sum;
}
