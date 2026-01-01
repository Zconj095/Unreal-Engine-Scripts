#include "NeuralLunarSynchronizationComponent.h"

#include "Math/UnrealMathUtility.h"
#include "UObject/Package.h"

namespace
{
	static constexpr int32 LocalInputCount = 2;
	static constexpr int32 LocalHiddenNeurons = 5;
	static constexpr int32 LocalOutputCount = 1;

	static constexpr double HiddenWeightInit[LocalHiddenNeurons][LocalInputCount] =
	{
		{0.15, -0.11},
		{-0.09, 0.21},
		{0.13, 0.04},
		{-0.02, -0.07},
		{0.08, 0.17},
	};

	static constexpr double HiddenBiasInit[LocalHiddenNeurons] =
	{
		0.01, -0.02, 0.03, 0.0, 0.01,
	};

	static constexpr double OutputWeightInit[LocalHiddenNeurons] =
	{
		0.25, -0.18, 0.22, 0.05, 0.12,
	};

	static constexpr double OutputBiasInit[LocalOutputCount] =
	{
		0.03,
	};
}

UNeuralLunarSynchronizationComponent::UNeuralLunarSynchronizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HiddenWeights.Reserve(HiddenNeurons * InputCount);
	for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
	{
		for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
		{
			HiddenWeights.Add(HiddenWeightInit[HiddenIndex][InputIndex]);
		}
	}

	HiddenBiases.Append(HiddenBiasInit, HiddenNeurons);
	OutputWeights.Append(OutputWeightInit, HiddenNeurons);
	OutputBiases.Append(OutputBiasInit, OutputCount);
}

double UNeuralLunarSynchronizationComponent::Sigmoid(double Value)
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}

double UNeuralLunarSynchronizationComponent::PredictLunarPower(double TimeSeconds, double PhiValue) const
{
	const double InputValues[InputCount] = { TimeSeconds, PhiValue };
	double HiddenOutputs[HiddenNeurons];

	for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
	{
		double Accumulator = HiddenBiases[HiddenIndex];
		const int32 RowStart = HiddenIndex * InputCount;

		for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
		{
			Accumulator += HiddenWeights[RowStart + InputIndex] * InputValues[InputIndex];
		}

		HiddenOutputs[HiddenIndex] = Sigmoid(Accumulator);
	}

	double FinalAccumulator = OutputBiases[0];
	for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
	{
		FinalAccumulator += OutputWeights[HiddenIndex] * HiddenOutputs[HiddenIndex];
	}

	return FinalAccumulator;
}

void UNeuralLunarSynchronizationComponent::LogSynchronizationTest()
{
	constexpr double TestTime = 5.0;
	const double TestPhi = FMath::Sin(TestTime);

	if (UNeuralLunarSynchronizationComponent* TestNetwork = NewObject<UNeuralLunarSynchronizationComponent>(GetTransientPackage()))
	{
		const double Power = TestNetwork->PredictLunarPower(TestTime, TestPhi);
		UE_LOG(LogTemp, Log, TEXT("Predicted Power Transfer (Neural): %.4f"), Power);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to instantiate Neural Lunar Synchronization component for the test."));
	}
}
