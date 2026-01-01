#include "NeuralSolarIntensityModelComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeuralSolarIntensityModelComponent::UNeuralSolarIntensityModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralSolarIntensityModelComponent::BeginPlay()
{
	Super::BeginPlay();

	RandomizeNetwork();
	TestPrediction();
}

float UNeuralSolarIntensityModelComponent::Predict(float InPowerOutput, float InDistance) const
{
	if (HiddenNeuronCount <= 0)
	{
		return 0.0f;
	}

	const float Inputs[2] = { InPowerOutput, InDistance };
	float Output = OutputBias;

	for (int32 NeuronIndex = 0; NeuronIndex < HiddenNeuronCount; ++NeuronIndex)
	{
		const int32 WeightStart = NeuronIndex * 2;
		float Sum = 0.0f;

		if (HiddenWeights.IsValidIndex(WeightStart + 0))
		{
			Sum += HiddenWeights[WeightStart + 0] * Inputs[0];
		}

		if (HiddenWeights.IsValidIndex(WeightStart + 1))
		{
			Sum += HiddenWeights[WeightStart + 1] * Inputs[1];
		}

		if (HiddenBiases.IsValidIndex(NeuronIndex))
		{
			Sum += HiddenBiases[NeuronIndex];
		}

		const float ActivationValue = DenseReLU(Sum);
		if (OutputWeights.IsValidIndex(NeuronIndex))
		{
			Output += OutputWeights[NeuronIndex] * ActivationValue;
		}
	}

	return Output;
}

void UNeuralSolarIntensityModelComponent::RandomizeNetwork()
{
	EnsureNetworkArrays();

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

void UNeuralSolarIntensityModelComponent::TestPrediction()
{
	PredictedIntensity = Predict(PowerOutput, Distance);
	UE_LOG(LogTemp, Log, TEXT("Predicted Solar Intensity (Neural): %0.4f W/m²"), PredictedIntensity);
}

float UNeuralSolarIntensityModelComponent::DenseReLU(float Value) const
{
	return FMath::Max(0.0f, Value);
}

void UNeuralSolarIntensityModelComponent::EnsureNetworkArrays() const
{
	const int32 TotalHiddenWeights = HiddenNeuronCount * 2;
	const_cast<TArray<float>&>(HiddenWeights).SetNumZeroed(TotalHiddenWeights);
	const_cast<TArray<float>&>(HiddenBiases).SetNumZeroed(HiddenNeuronCount);
	const_cast<TArray<float>&>(OutputWeights).SetNumZeroed(HiddenNeuronCount);
}
