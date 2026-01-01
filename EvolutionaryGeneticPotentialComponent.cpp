#include "EvolutionaryGeneticPotentialComponent.h"
#include "Math/UnrealMathUtility.h"

UEvolutionaryGeneticPotentialComponent::UEvolutionaryGeneticPotentialComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	XMin = 0.0f;
	XMax = 10.0f;
	NumSteps = 100;
	StartTime = 0.0f;
	EndTime = 1.0f;
}

void UEvolutionaryGeneticPotentialComponent::BeginPlay()
{
	Super::BeginPlay();

	const float EGP = CalculateEGP(XMin, XMax, NumSteps, StartTime, EndTime);
	UE_LOG(LogTemp, Log, TEXT("Evolutionary Genetic Potential (EGP): %f"), EGP);
}

float UEvolutionaryGeneticPotentialComponent::CalculateEGP(float InXMin, float InXMax, int32 Steps, float InStart, float InEnd) const
{
	if (Steps <= 0 || FMath::IsNearlyZero(InXMax - InXMin))
	{
		return 0.0f;
	}
	const float DX = (InXMax - InXMin) / Steps;
	float Sum = 0.0f;
	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float X = InXMin + Index * DX;
		const float WaveValue = WaveFunction(X, InEnd);
		const float ConjValue = ConjugateWaveFunction(X, InEnd);
		Sum += ConjValue * WaveValue * DX;
	}
	return Sum;
}

float UEvolutionaryGeneticPotentialComponent::WaveFunction(float X, float T) const
{
	return FMath::Sin(X + T);
}

float UEvolutionaryGeneticPotentialComponent::ConjugateWaveFunction(float X, float T) const
{
	return FMath::Sin(X + T);
}
