#include "NeurochemicalEnergyIntegrationComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeurochemicalEnergyIntegrationComponent::UNeurochemicalEnergyIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeurochemicalEnergyIntegrationComponent::BeginPlay()
{
	Super::BeginPlay();

	NeurochemicalEnergy = CalculateNeurochemicalEnergy();
	UE_LOG(LogTemp, Log, TEXT("Neurochemical Energy Integrated (Eₙ): %0.4f"), NeurochemicalEnergy);
}

float UNeurochemicalEnergyIntegrationComponent::CalculateNeurochemicalEnergy() const
{
	if (NumSamples <= 0 || FMath::IsNearlyEqual(StartTime, EndTime))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration bounds or zero samples."));
		return 0.0f;
	}

	const float Delta = (EndTime - StartTime) / static_cast<float>(NumSamples);
	float Integral = 0.0f;

	for (int32 Index = 0; Index < NumSamples; ++Index)
	{
		const float CurrentTime = StartTime + Index * Delta;
		const float Flux = FMath::Sin(CurrentTime);
		Integral += FreeEnergyChange * Flux * Delta;
	}

	return Integral;
}
