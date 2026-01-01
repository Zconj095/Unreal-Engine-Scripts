#include "QuantumLeylineEnergyComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UQuantumLeylineEnergyComponent::UQuantumLeylineEnergyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumLeylineEnergyComponent::BeginPlay()
{
	Super::BeginPlay();

	if (TimeStep <= 0.0f || TimeEnd <= TimeStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid time parameters."));
		return;
	}

	float Total = 0.0f;
	for (float t = TimeStart; t < TimeEnd; t += TimeStep)
	{
		const float PhiVal = FMath::Sin(t) + 1.0f;
		const float GradVal = FMath::Cos(t) + 1.0f;
		Total += PhiVal * GradVal * TimeStep;
	}

	QuantumEnergyFlow = Total;
	UE_LOG(LogTemp, Log, TEXT("Quantum Energy Transfer (P_quantum): %0.4f"), QuantumEnergyFlow);
}
