#include "NeurochemicalPathwayComplexityComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeurochemicalPathwayComplexityComponent::UNeurochemicalPathwayComplexityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeurochemicalPathwayComplexityComponent::BeginPlay()
{
	Super::BeginPlay();

	NeurochemicalPathwayComplexityValue = CalculateNPC();
	UE_LOG(LogTemp, Log, TEXT("Neurochemical Pathway Complexity (NPC): %0.4f"), NeurochemicalPathwayComplexityValue);
}

float UNeurochemicalPathwayComplexityComponent::CalculateNPC() const
{
	if (Concentrations.Num() != PathwayLengths.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Concentrations and pathway lengths must share the same length."));
		return 0.0f;
	}

	float Accumulated = 0.0f;
	for (int32 Index = 0; Index < Concentrations.Num(); ++Index)
	{
		const float Length = PathwayLengths[Index];
		if (Length <= 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pathway length at index %d must be greater than zero."), Index);
			continue;
		}

		Accumulated += Concentrations[Index] * FMath::Log2(Length);
	}

	return Alpha * Accumulated;
}
