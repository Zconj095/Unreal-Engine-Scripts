#include "EnzymaticInteractionComprehensionComponent.h"
#include "Math/UnrealMathUtility.h"

UEnzymaticInteractionComprehensionComponent::UEnzymaticInteractionComprehensionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Gamma = 1.0f;
	DeltaEActive = 10.0f;
	TimeMs = 1.0f;
	CognitiveEnergyExpenditure = 0.1f;
	Result = 0.0f;
}

void UEnzymaticInteractionComprehensionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FMath::IsNearlyZero(TimeMs) || FMath::IsNearlyZero(CognitiveEnergyExpenditure))
	{
		UE_LOG(LogTemp, Error, TEXT("T and µ must not be zero to calculate EICI."));
		Result = 0.0f;
	}
	else
	{
		Result = CalculateEICI(Gamma, DeltaEActive, TimeMs, CognitiveEnergyExpenditure);
	}

	LogResult();
}

float UEnzymaticInteractionComprehensionComponent::CalculateEICI(float InGamma, float InDeltaE, float InTime, float InMu) const
{
	return InGamma * (InDeltaE / (InTime * InMu));
}

void UEnzymaticInteractionComprehensionComponent::LogResult() const
{
	UE_LOG(LogTemp, Log, TEXT("EICI Result: %.2f"), Result);
}
