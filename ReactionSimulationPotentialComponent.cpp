#include "ReactionSimulationPotentialComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UReactionSimulationPotentialComponent::UReactionSimulationPotentialComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UReactionSimulationPotentialComponent::BeginPlay()
{
	Super::BeginPlay();
	Potential = CalculatePotential();
	UE_LOG(LogTemp, Log, TEXT("Reaction Simulation Potential (Pr): %0.2f"), Potential);
}

void UReactionSimulationPotentialComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const float NewPotential = CalculatePotential();
	if (!FMath::IsNearlyEqual(Potential, NewPotential))
	{
		Potential = NewPotential;
		UE_LOG(LogTemp, Log, TEXT("Reaction Simulation Potential (Pr): %0.2f"), Potential);
	}
}

float UReactionSimulationPotentialComponent::CalculatePotential() const
{
	if (StepSize <= 0.0f || CurrentTime < StartTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration parameters for Reaction Simulation Potential."));
		return 0.0f;
	}

	float Result = 0.0f;
	for (float X = StartTime; X <= CurrentTime; X += StepSize)
	{
		const float Wavefunction = FMath::Exp(-X * X / (2.0f * CurrentTime * CurrentTime)) / FMath::Sqrt(2.0f * PI * CurrentTime * CurrentTime);
		const float Derivative = (FMath::Exp(-(X + SMALL_NUMBER) * (X + SMALL_NUMBER) / (2.0f * CurrentTime * CurrentTime)) - Wavefunction) / SMALL_NUMBER;
		const float Hamiltonian = 0.5f * X * X - 0.5f * FMath::Pow(Derivative, 2);
		Result += Wavefunction * Hamiltonian * Wavefunction * StepSize;
	}

	return Result;
}
