#include "EmpathyIntegrationComponent.h"

UEmpathyIntegrationComponent::UEmpathyIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	StartTime = 0.0f;
	EndTime = 10.0f;
	IntegrationSteps = 1000;
	CognitiveStimuli = 1.0f;
	EmotionalStimuli = 1.0f;
	EmpathyValue = 0.0f;
}

void UEmpathyIntegrationComponent::BeginPlay()
{
	Super::BeginPlay();

	EmpathyValue = CalculateEmpathyIntegration(StartTime, EndTime, IntegrationSteps);
	UE_LOG(LogTemp, Log, TEXT("Empathy Integration (E_i): %f"), EmpathyValue);
}

float UEmpathyIntegrationComponent::EnergyFluxFunction(float Sc, float Es) const
{
	return Sc * Es;
}

float UEmpathyIntegrationComponent::CalculateEmpathyIntegration(float T0, float T, int32 Steps) const
{
	if (Steps <= 0 || T <= T0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration parameters for EmpathyIntegrationComponent."));
		return 0.0f;
	}

	const float Dt = (T - T0) / Steps;
	float Sum = 0.0f;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float CurrentTime = T0 + Index * Dt;
		const float Phi = EnergyFluxFunction(CognitiveStimuli, EmotionalStimuli);
		Sum += Phi * Dt;
	}

	return Sum;
}
