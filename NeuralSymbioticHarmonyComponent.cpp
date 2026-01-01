#include "NeuralSymbioticHarmonyComponent.h"
#include "Logging/LogMacros.h"

UNeuralSymbioticHarmonyComponent::UNeuralSymbioticHarmonyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralSymbioticHarmonyComponent::BeginPlay()
{
	Super::BeginPlay();

	CalculateNSH();
	UE_LOG(LogTemp, Log, TEXT("Neural Symbiotic Harmony (NSH): %0.4f"), NSHValue);
}

float UNeuralSymbioticHarmonyComponent::CalculateNSH()
{
	const int32 Count = SymbioticRelationships.Num();
	if (Count == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No symbiotic relationships provided. Add values before calculating NSH."));
		NSHValue = 0.0f;
		return NSHValue;
	}

	float Sum = 0.0f;
	for (float Strength : SymbioticRelationships)
	{
		Sum += Strength;
	}

	NSHValue = Xi * (Sum / FMath::Sqrt(static_cast<float>(Count)));
	return NSHValue;
}

void UNeuralSymbioticHarmonyComponent::AddRelationship(float Strength)
{
	SymbioticRelationships.Add(Strength);
}
