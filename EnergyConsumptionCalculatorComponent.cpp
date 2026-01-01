#include "EnergyConsumptionCalculatorComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyConsumptionCalculatorComponent::UEnergyConsumptionCalculatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	BaselineEnergy = 1.0f;
	QuantumEnergy = 1.0f;
	Frequency = 1.0f;
	OperationTime = 1.0f;
	EnergyConsumption = 0.0f;
}

void UEnergyConsumptionCalculatorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnergyConsumptionCalculatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	EnergyConsumption = CalculateEnergyConsumption(BaselineEnergy, QuantumEnergy, Frequency, OperationTime);
	UE_LOG(LogTemp, Log, TEXT("Energy Consumption: %f"), EnergyConsumption);
}

float UEnergyConsumptionCalculatorComponent::CalculateEnergyConsumption(float Pb, float Pq, float Omega, float T) const
{
	const int32 NumSteps = 1000;
	if (T <= 0.0f || NumSteps <= 0)
	{
		return 0.0f;
	}

	const float DeltaT = T / static_cast<float>(NumSteps);
	float TotalEnergy = 0.0f;

	for (int32 Index = 0; Index < NumSteps; ++Index)
	{
		const float Time = Index * DeltaT;
		const float EnergyAtT = Pb + Pq * FMath::Sin(Omega * Time);
		TotalEnergy += EnergyAtT * DeltaT;
	}

	return TotalEnergy;
}
