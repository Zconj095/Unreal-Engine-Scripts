#include "PlasmaNeuralModelComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPlasmaNeuralModelComponent::UPlasmaNeuralModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlasmaNeuralModelComponent::BeginPlay()
{
	Super::BeginPlay();
	RandomizeNetwork();
	TestPrediction();
}

TArray<float> UPlasmaNeuralModelComponent::Predict(const TArray<float>& InValues) const
{
	TArray<float> Output;
	Output.SetNumZeroed(OutputSize);

	if (HiddenLayerSize <= 0 || OutputSize <= 0)
	{
		return Output;
	}

	TArray<float> HiddenOutputs;
	HiddenOutputs.SetNumZeroed(HiddenLayerSize);

	for (int32 NeuronIdx = 0; NeuronIdx < HiddenLayerSize; ++NeuronIdx)
	{
		const int32 WeightIdx = NeuronIdx;
		float Sum = (HiddenWeights.IsValidIndex(WeightIdx) ? HiddenWeights[WeightIdx] * (InValues.IsValidIndex(NeuronIdx) ? InValues[NeuronIdx] : 0.0f) : 0.0f)
			+ (HiddenBiases.IsValidIndex(NeuronIdx) ? HiddenBiases[NeuronIdx] : 0.0f);
		HiddenOutputs[NeuronIdx] = Activation(Sum);
	}

	for (int32 OutputIdx = 0; OutputIdx < OutputSize; ++OutputIdx)
	{
		float Sum = (OutputBiases.IsValidIndex(OutputIdx) ? OutputBiases[OutputIdx] : 0.0f);
		for (int32 HiddenIdx = 0; HiddenIdx < HiddenLayerSize; ++HiddenIdx)
		{
			const int32 WeightIdx = OutputIdx * HiddenLayerSize + HiddenIdx;
			if (OutputWeights.IsValidIndex(WeightIdx))
			{
				Sum += OutputWeights[WeightIdx] * HiddenOutputs[HiddenIdx];
			}
		}
		Output[OutputIdx] = Sum;
	}

	return Output;
}

void UPlasmaNeuralModelComponent::RandomizeNetwork()
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
	for (float& Bias : OutputBiases)
	{
		Bias = Stream.FRandRange(-1.0f, 1.0f);
	}
}

void UPlasmaNeuralModelComponent::TestPrediction()
{
	if (Inputs.Num() == 0)
	{
		return;
	}

	PredictedVelocity = Predict(Inputs);
	UE_LOG(LogTemp, Log, TEXT("Predicted Plasma Velocity: %s"), *FString::JoinBy(PredictedVelocity, TEXT(", "), [](float Value) { return FString::SanitizeFloat(Value); }));
}

void UPlasmaNeuralModelComponent::EnsureNetwork() const
{
	const_cast<TArray<float>&>(HiddenWeights).SetNumZeroed(HiddenLayerSize);
	const_cast<TArray<float>&>(HiddenBiases).SetNumZeroed(HiddenLayerSize);
	const_cast<TArray<float>&>(OutputWeights).SetNumZeroed(OutputSize * HiddenLayerSize);
	const_cast<TArray<float>&>(OutputBiases).SetNumZeroed(OutputSize);
}

float UPlasmaNeuralModelComponent::Activation(float Value) const
{
	return 1.0f / (1.0f + FMath::Exp(-Value));
}
