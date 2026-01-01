#include "NeuralTidalForceComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeuralTidalForceComponent::UNeuralTidalForceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralTidalForceComponent::BeginPlay()
{
	Super::BeginPlay();

	RandomizeNetwork();
	TestPrediction();
}

float UNeuralTidalForceComponent::Predict(float MassEarth, float MassMoon, float Distance) const
{
	if (HiddenNeuronCount <= 0)
	{
		return 0.0f;
	}

	const float Inputs[3] = { MassEarth, MassMoon, Distance };
	float SumOutput = OutputBias;

	for (int32 NeuronIdx = 0; NeuronIdx < HiddenNeuronCount; ++NeuronIdx)
	{
		const int32 WeightIndex = NeuronIdx * 3;
		float Sum = 0.0f;

		for (int32 InputIdx = 0; InputIdx < 3; ++InputIdx)
		{
			const int32 ArrayIndex = WeightIndex + InputIdx;
			if (HiddenWeights.IsValidIndex(ArrayIndex))
			{
				Sum += HiddenWeights[ArrayIndex] * Inputs[InputIdx];
			}
		}

		if (HiddenBiases.IsValidIndex(NeuronIdx))
		{
			Sum += HiddenBiases[NeuronIdx];
		}

		const float ActivationValue = Activation(Sum);
		if (OutputWeights.IsValidIndex(NeuronIdx))
		{
			SumOutput += OutputWeights[NeuronIdx] * ActivationValue;
		}
	}

	return SumOutput;
}

void UNeuralTidalForceComponent::RandomizeNetwork()
{
	EnsureNetwork();

	FRandomStream Stream(FDateTime::Now().GetTicks());
	for (float& Weight : HiddenWeights)
	{
		Weight = Stream.FRandRange(-1.0f, 1.0f);
	}

	for (float& Bias : HiddenBiases)
	{
		Bias = Stream.FRandRange(-1.0f, 1.0f);
	}

	for (float& Weight : OutputWeights)
	{
		Weight = Stream.FRandRange(-1.0f, 1.0f);
	}

	OutputBias = Stream.FRandRange(-1.0f, 1.0f);
}

void UNeuralTidalForceComponent::TestPrediction()
{
	PredictedForce = Predict(5.972e24f, 7.348e22f, 3.844e8f);
	UE_LOG(LogTemp, Log, TEXT("Predicted Tidal Force: %0.2f N"), PredictedForce);
}

void UNeuralTidalForceComponent::EnsureNetwork()
{
	const int32 TotalHiddenWeights = HiddenNeuronCount * 3;
	if (HiddenWeights.Num() != TotalHiddenWeights)
	{
		HiddenWeights.SetNumZeroed(TotalHiddenWeights);
	}

	if (HiddenBiases.Num() != HiddenNeuronCount)
	{
		HiddenBiases.SetNumZeroed(HiddenNeuronCount);
	}

	if (OutputWeights.Num() != HiddenNeuronCount)
	{
		OutputWeights.SetNumZeroed(HiddenNeuronCount);
	}
}

float UNeuralTidalForceComponent::Activation(float Value) const
{
	return 1.0f / (1.0f + FMath::Exp(-Value));
}
