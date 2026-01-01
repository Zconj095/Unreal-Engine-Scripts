#include "HumanEmpressEvolutionaryNeuralComplexityComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

UHumanEmpressEvolutionaryNeuralComplexityComponent::UHumanEmpressEvolutionaryNeuralComplexityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Enc = 0.0f;
}

void UHumanEmpressEvolutionaryNeuralComplexityComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateComplexity();
}

void UHumanEmpressEvolutionaryNeuralComplexityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateComplexity();
}

void UHumanEmpressEvolutionaryNeuralComplexityComponent::UpdateComplexity()
{
	EnsureDataAligned();
	if (Probabilities.Num() != PathwayLengths.Num())
	{
		Enc = 0.0f;
		return;
	}

	Enc = CalculateENC();
	UE_LOG(LogTemp, Log, TEXT("ENC: %.4f"), Enc);
}

float UHumanEmpressEvolutionaryNeuralComplexityComponent::CalculateENC() const
{
	float Sum = 0.0f;
	for (int32 Index = 0; Index < Probabilities.Num(); ++Index)
	{
		const float Probability = Probabilities[Index];
		const float PathLength = PathwayLengths[Index];
		if (Probability > 0.0f && PathLength > 0.0f)
		{
			Sum += Probability * FMath::Log2(PathLength);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid pathway data at index %d."), Index);
		}
	}

	return Kappa * Sum;
}

void UHumanEmpressEvolutionaryNeuralComplexityComponent::EnsureDataAligned()
{
	if (Probabilities.Num() != PathwayLengths.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Probabilities and pathway lengths must be the same size."));
	}
}

void UHumanEmpressEvolutionaryNeuralComplexityComponent::UpdateProbabilities(const TArray<float>& NewProbabilities)
{
	Probabilities = NewProbabilities;
}

void UHumanEmpressEvolutionaryNeuralComplexityComponent::UpdatePathwayLengths(const TArray<float>& NewPathwayLengths)
{
	PathwayLengths = NewPathwayLengths;
}
