#include "FantasyNeurochemicalSynthesisEfficiencyComponent.h"
#include "Math/UnrealMathUtility.h"

UFantasyNeurochemicalSynthesisEfficiencyComponent::UFantasyNeurochemicalSynthesisEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Zeta = 1.5f;
	NeuronsFantasy = 5000.0f;
	TotalNeurons = 10000.0f;
	FNSEValue = 0.0f;
}

void UFantasyNeurochemicalSynthesisEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	FNSEValue = CalculateFNSE(Zeta, NeuronsFantasy, TotalNeurons);
	UE_LOG(LogTemp, Log, TEXT("Fantasy Neurochemical Synthesis Efficiency (FNSE): %f"), FNSEValue);
}

float UFantasyNeurochemicalSynthesisEfficiencyComponent::CalculateFNSE(float InZeta, float InNeuronsFantasy, float InTotalNeurons) const
{
	if (FMath::IsNearlyZero(InTotalNeurons))
	{
		UE_LOG(LogTemp, Error, TEXT("Total neurons (N_total) cannot be zero."));
		return 0.0f;
	}

	float Ratio = InNeuronsFantasy / InTotalNeurons;
	return InZeta * FMath::Log2(1.0f + Ratio);
}
