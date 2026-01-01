#include "NeuroevolutionaryPlasticityComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeuroevolutionaryPlasticityComponent::UNeuroevolutionaryPlasticityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuroevolutionaryPlasticityComponent::BeginPlay()
{
	Super::BeginPlay();

	NeuroevolutionaryPlasticityValue = CalculateNEP();
	UE_LOG(LogTemp, Log, TEXT("Neuroevolutionary Plasticity (P_e): %0.4f"), NeuroevolutionaryPlasticityValue);
}

float UNeuroevolutionaryPlasticityComponent::CalculateNEP() const
{
	if (IntegrationSteps <= 0 || FMath::IsNearlyEqual(StartTime, EndTime))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration configuration."));
		return 0.0f;
	}

	const float DeltaTime = (EndTime - StartTime) / static_cast<float>(IntegrationSteps);
	const float StimuliSum = EnvironmentalStimuli + GeneticInfluences;
	float Sum = 0.0f;

	for (int32 Index = 0; Index < IntegrationSteps; ++Index)
	{
		Sum += StimuliSum * DeltaTime;
	}

	return PlasticityConstant * Sum;
}
