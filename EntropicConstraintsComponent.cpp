#include "EntropicConstraintsComponent.h"
#include "Math/UnrealMathUtility.h"

UEntropicConstraintsComponent::UEntropicConstraintsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InformationEntropy = 1.0f;
	EntropyChange = 0.5f;
	EntropicEfficiency = 0.0f;
}

void UEntropicConstraintsComponent::BeginPlay()
{
	Super::BeginPlay();

	EntropicEfficiency = CalculateEntropicEfficiency(InformationEntropy, EntropyChange);
	UE_LOG(LogTemp, Log, TEXT("Neural Entropic Efficiency (η_s): %f"), EntropicEfficiency);
}

float UEntropicConstraintsComponent::CalculateEntropicEfficiency(float H, float DeltaS) const
{
	if (FMath::IsNearlyZero(DeltaS))
	{
		UE_LOG(LogTemp, Warning, TEXT("Entropy change (ΔS) is zero. Cannot compute efficiency."));
		return 0.0f;
	}
	return H / DeltaS;
}
