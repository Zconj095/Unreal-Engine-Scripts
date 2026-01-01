#include "EntropyCalculatorComponent.h"
#include "Math/UnrealMathUtility.h"

UEntropyCalculatorComponent::UEntropyCalculatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Probabilities = {0.2, 0.3, 0.1, 0.4};
	EntropyValue = 0.0;
}

void UEntropyCalculatorComponent::BeginPlay()
{
	Super::BeginPlay();

	EntropyValue = ComputeEntropy(Probabilities);
	LogEntropy();
}

double UEntropyCalculatorComponent::ComputeEntropy(const TArray<double>& InProbabilities) const
{
	double Entropy = 0.0;
	for (double Probability : InProbabilities)
	{
		if (Probability <= 0.0)
		{
			continue;
		}
		Entropy += -Probability * FMath::LogX(2.0, Probability);
	}
	return Entropy;
}

void UEntropyCalculatorComponent::LogEntropy() const
{
	UE_LOG(LogTemp, Log, TEXT("Entropy: %.4f"), EntropyValue);
}
