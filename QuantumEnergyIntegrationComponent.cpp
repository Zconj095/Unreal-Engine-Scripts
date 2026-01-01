#include "QuantumEnergyIntegrationComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

static const float HBAR = 1.054e-34f;

UQuantumEnergyIntegrationComponent::UQuantumEnergyIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumEnergyIntegrationComponent::BeginPlay()
{
	Super::BeginPlay();

	QuantumEnergy = CalculateQuantumEnergy();
	UE_LOG(LogTemp, Log, TEXT("Quantum Energy Integrated (E_q): %0.4e J"), QuantumEnergy);
}

float UQuantumEnergyIntegrationComponent::CalculateQuantumEnergy() const
{
	if (IntegrationSteps <= 0 || EndTime <= StartTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration parameters."));
		return 0.0f;
	}

	const float Dt = (EndTime - StartTime) / IntegrationSteps;
	const float Energy = HBAR * AngularFrequency;
	return Energy * Dt * IntegrationSteps;
}
