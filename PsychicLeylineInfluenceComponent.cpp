#include "PsychicLeylineInfluenceComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPsychicLeylineInfluenceComponent::UPsychicLeylineInfluenceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPsychicLeylineInfluenceComponent::BeginPlay()
{
	Super::BeginPlay();

	PsychicEnergyFlow = CalculatePsychicEnergyFlow();
	UE_LOG(LogTemp, Log, TEXT("Psychic Energy Flow (P_psychic): %0.4f"), PsychicEnergyFlow);
}

float UPsychicLeylineInfluenceComponent::CalculatePsychicEnergyFlow() const
{
	if (TimeStep <= 0.0f || TimeEnd <= TimeStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid time parameters."));
		return 0.0f;
	}

	float Total = 0.0f;
	for (float t = TimeStart; t < TimeEnd; t += TimeStep)
	{
		const float PhiVal = EvaluatePhi(t);
		const float GradVal = EvaluateGradPsi(t);
		Total += PhiVal * GradVal * TimeStep;
	}

	return Total;
}

float UPsychicLeylineInfluenceComponent::EvaluatePhi(float Time) const
{
	return FMath::Sin(Time) + 1.0f;
}

float UPsychicLeylineInfluenceComponent::EvaluateGradPsi(float Time) const
{
	return FMath::Cos(Time) + 1.0f;
}
