#include "HMMQuantumIntegrationComponent.h"
#include "QuantumSystemManager.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformTime.h"
#include "Engine/World.h"

UHMMQuantumIntegrationComponent::UHMMQuantumIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHMMQuantumIntegrationComponent::InitializeQuantumSystem(int32 NumStates, const TArray<float>& InitialState)
{
	if (!QuantumManager)
	{
		QuantumManager = NewObject<UQuantumSystemManager>(this);
	}

	if (QuantumManager)
	{
		TArray<double> Converted;
		Converted.Reserve(InitialState.Num());
		for (float Value : InitialState)
		{
			Converted.Add(static_cast<double>(Value));
		}

		QuantumManager->InitializeState(Converted);
	}
}

void UHMMQuantumIntegrationComponent::SetTransitionMatrix(const TArray<float>& TransitionMatrix, int32 NumStates)
{
	if (QuantumManager)
	{
		TArray<double> Converted;
		Converted.Reserve(TransitionMatrix.Num());
		for (float Value : TransitionMatrix)
		{
			Converted.Add(static_cast<double>(Value));
		}

		QuantumManager->SetTransitionMatrix(Converted, NumStates);
	}
}

void UHMMQuantumIntegrationComponent::SetEmissionMatrix(const TArray<float>& EmissionMatrix, int32 NumStates)
{
	if (QuantumManager)
	{
		TArray<double> Converted;
		Converted.Reserve(EmissionMatrix.Num());
		for (float Value : EmissionMatrix)
		{
			Converted.Add(static_cast<double>(Value));
		}

		QuantumManager->SetEmissionMatrix(Converted, NumStates);
	}
}

void UHMMQuantumIntegrationComponent::RunSimulation(int32 Steps)
{
	if (!QuantumManager || Steps <= 0)
	{
		return;
	}

	for (int32 Step = 0; Step < Steps; ++Step)
	{
		QuantumManager->ApplyTransition();
		const int32 Observation = QuantumManager->Measure();
		UE_LOG(LogTemp, Log, TEXT("Step %d: Observation = %d"), Step + 1, Observation);
	}
}
