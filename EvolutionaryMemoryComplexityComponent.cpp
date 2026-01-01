#include "EvolutionaryMemoryComplexityComponent.h"
#include "Math/UnrealMathUtility.h"

UEvolutionaryMemoryComplexityComponent::UEvolutionaryMemoryComplexityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Beta = 1.0f;
	Concepts.Add({0.5f, 1.0f});
	Concepts.Add({0.3f, 2.0f});
	Concepts.Add({0.2f, 4.0f});
}

void UEvolutionaryMemoryComplexityComponent::BeginPlay()
{
	Super::BeginPlay();

	const float EMC = CalculateEMC();
	UE_LOG(LogTemp, Log, TEXT("Evolutionary Memory Complexity (EMC): %f"), EMC);
}

float UEvolutionaryMemoryComplexityComponent::CalculateEMC() const
{
	if (Concepts.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No evolutionary concepts supplied."));
		return 0.0f;
	}

	float Sum = 0.0f;
	for (const FEvolutionaryConcept& Concept : Concepts)
	{
		if (Concept.Complexity > 0.0f)
		{
			Sum += Concept.RecallProbability * FMath::LogX(2.0f, Concept.Complexity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Concept complexity must be > 0."));
		}
	}

	return Beta * Sum;
}
