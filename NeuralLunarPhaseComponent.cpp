#include "NeuralLunarPhaseComponent.h"

#include "Math/UnrealMathUtility.h"
#include "UObject/Package.h"

namespace
{
	static constexpr int32 LocalInputCount = 2;
	static constexpr int32 LocalHiddenNeurons = 10;
	static constexpr int32 LocalOutputCount = 1;

	static constexpr double HiddenWeightInit[LocalHiddenNeurons][LocalInputCount] =
	{
		{-0.01, 0.01},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0},
	};

	static constexpr double HiddenBiasInit[LocalHiddenNeurons] =
	{
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	};

	static constexpr double OutputWeightInit[LocalHiddenNeurons] =
	{
		400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	};

	static constexpr double OutputBiasInit[LocalOutputCount] =
	{
		-200.0,
	};
}

UNeuralLunarPhaseComponent::UNeuralLunarPhaseComponent()
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

double UNeuralLunarPhaseComponent::Sigmoid(double Value)
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}

double UNeuralLunarPhaseComponent::PredictLunarPhase(double ThetaSunRadians, double ThetaMoonRadians) const
{
	const double InputValues[InputCount] = { ThetaSunRadians, ThetaMoonRadians };
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

void UNeuralLunarPhaseComponent::LogPhaseTest()
{
	constexpr double TestThetaSun = PI / 6.0;
	constexpr double TestThetaMoon = PI / 3.0;

	if (UNeuralLunarPhaseComponent* TestNetwork = NewObject<UNeuralLunarPhaseComponent>(GetTransientPackage()))
	{
		const double PhaseAngle = TestNetwork->PredictLunarPhase(TestThetaSun, TestThetaMoon);
		UE_LOG(LogTemp, Log, TEXT("Predicted Lunar Phase Angle (Neural): %.4f radians"), PhaseAngle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to instantiate Neural Lunar Phase component for the phase test."));
	}
}
