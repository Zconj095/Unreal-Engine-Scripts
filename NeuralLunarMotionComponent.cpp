#include "NeuralLunarMotionComponent.h"

#include "Math/UnrealMathUtility.h"
#include "UObject/Package.h"

namespace
{
	static constexpr int32 LocalInputCount = 3;
	static constexpr int32 LocalHiddenNeurons = 10;
	static constexpr int32 LocalOutputCount = 1;

	static constexpr double HiddenWeightInit[LocalHiddenNeurons][LocalInputCount] =
	{
		{0.00045, -0.00012, 0.00032},
		{0.00052, 0.00031, -0.00018},
		{-0.00027, 0.00058, 0.00041},
		{0.00063, 0.00011, 0.00005},
		{0.00044, -0.00021, 0.00023},
		{0.00049, 0.00007, 0.00037},
		{0.00030, 0.00045, -0.00011},
		{0.00018, 0.00054, 0.00009},
		{-0.00012, 0.00039, 0.00026},
		{0.00057, 0.00029, -0.00022},
	};

	static constexpr double HiddenBiasInit[LocalHiddenNeurons] =
	{
		0.12, -0.05, 0.03, 0.08, -0.02, 0.04, 0.06, -0.01, 0.11, 0.07,
	};

	static constexpr double OutputWeightInit[LocalHiddenNeurons] =
	{
		0.0023, 0.0028, -0.0011, 0.0019, 0.0021, 0.0025, 0.0017, -0.0009, 0.0015, 0.0026,
	};

	static constexpr double OutputBiasInit[LocalOutputCount] =
	{
		384400.0,
	};
}

UNeuralLunarMotionComponent::UNeuralLunarMotionComponent()
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

double UNeuralLunarMotionComponent::Sigmoid(double Value)
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}

double UNeuralLunarMotionComponent::PredictDistance(double SemiMajorAxisKm, double Eccentricity, double ThetaRadians) const
{
	const double InputValues[InputCount] = { SemiMajorAxisKm, Eccentricity, ThetaRadians };
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

void UNeuralLunarMotionComponent::LogDistanceTest()
{
	constexpr double TestSemiMajorAxis = 384400.0;
	constexpr double TestEccentricity = 0.0549;
	constexpr double TestTheta = PI / 4.0;

	if (UNeuralLunarMotionComponent* TestNetwork = NewObject<UNeuralLunarMotionComponent>(GetTransientPackage()))
	{
		const double Distance = TestNetwork->PredictDistance(TestSemiMajorAxis, TestEccentricity, TestTheta);
		UE_LOG(LogTemp, Log, TEXT("Predicted Distance to Moon: %.2f km"), Distance);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to instantiate Neural Lunar Motion component for the distance test."));
	}
}
