#include "NeuralSolarCycleModelComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeuralSolarCycleModelComponent::UNeuralSolarCycleModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralSolarCycleModelComponent::BeginPlay()
{
	Super::BeginPlay();

	RandomizeNetwork();
	PredictedAmplitude = Predict(MaxAmplitude, CyclePeriod, SampleTime);
	UE_LOG(LogTemp, Log, TEXT("Predicted Solar Activity Amplitude (Neural): %0.4f"), PredictedAmplitude);
}

float UNeuralSolarCycleModelComponent::Predict(float A0, float T, float Time) const
{
	if (HiddenNeuronCount <= 0)
	{
		return 0.0f;
	}

	const float Inputs[3] = { A0, T, Time };
	float Output = OutputBias;

	for (int32 NeuronIndex = 0; NeuronIndex < HiddenNeuronCount; ++NeuronIndex)
	{
		const int32 WeightStart = NeuronIndex * 3;
		const float Sum =
			  HiddenWeights.IsValidIndex(WeightStart + 0) ? HiddenWeights[WeightStart + 0] * Inputs[0] : 0.0f
			+ HiddenWeights.IsValidIndex(WeightStart + 1) ? HiddenWeights[WeightStart + 1] * Inputs[1] : 0.0f
			+ HiddenWeights.IsValidIndex(WeightStart + 2) ? HiddenWeights[WeightStart + 2] * Inputs[2] : 0.0f
			+ (HiddenBiases.IsValidIndex(NeuronIndex) ? HiddenBiases[NeuronIndex] : 0.0f);

		const float ActivationValue = Activation(Sum);
		Output += (OutputWeights.IsValidIndex(NeuronIndex) ? OutputWeights[NeuronIndex] : 0.0f) * ActivationValue;
	}

	return Output;
}

void UNeuralSolarCycleModelComponent::RandomizeNetwork()
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

void UNeuralSolarCycleModelComponent::TestPrediction()
{
	PredictedAmplitude = Predict(MaxAmplitude, CyclePeriod, SampleTime);
	UE_LOG(LogTemp, Log, TEXT("Predicted Solar Activity Amplitude (Neural): %0.4f"), PredictedAmplitude);
}

float UNeuralSolarCycleModelComponent::Activation(float Value) const
{
	return 1.0f / (1.0f + FMath::Exp(-Value));
}

void UNeuralSolarCycleModelComponent::EnsureNetworkArrays()
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
