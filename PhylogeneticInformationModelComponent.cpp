#include "PhylogeneticInformationModelComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPhylogeneticInformationModelComponent::UPhylogeneticInformationModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPhylogeneticInformationModelComponent::BeginPlay()
{
	Super::BeginPlay();

	InformationGain = ComputeInformationGain();
	UE_LOG(LogTemp, Log, TEXT("Phylogenetic Information Gain: %0.4f"), InformationGain);
}

float UPhylogeneticInformationModelComponent::ComputeInformationGain()
{
	if (CurrentProbabilities.Num() != AncestralProbabilities.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Probability arrays must have the same length."));
		return 0.0f;
	}

	float TotalGain = 0.0f;
	for (int32 Index = 0; Index < CurrentProbabilities.Num(); ++Index)
	{
		const float Current = CurrentProbabilities[Index];
		const float Ancestral = AncestralProbabilities[Index];
		if (Current > 0.0f && Ancestral > 0.0f)
		{
			TotalGain += Current * FMath::Loge(Current / Ancestral) / FMath::Loge(2.0f);
		}
	}

	InformationGain = TotalGain;
	return TotalGain;
}
