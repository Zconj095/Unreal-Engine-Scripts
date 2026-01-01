#include "NeuralMoonlightEnergyComponent.h"

#include "Math/UnrealMathUtility.h"
#include "UObject/Package.h"

namespace
{
	static constexpr int32 LocalInputCount = 3;
	static constexpr int32 LocalHiddenNeurons = 10;
	static constexpr int32 LocalOutputCount = 1;

	static constexpr double HiddenWeightInit[LocalHiddenNeurons][LocalInputCount] =
	{
		{0.0012, -0.0009, 0.0004},
		{-0.0005, 0.0011, 0.0002},
		{0.0007, 0.0003, -0.0008},
		{0.0010, -0.0002, 0.0009},
		{0.0006, 0.0005, 0.0001},
		{-0.0003, 0.0008, 0.0006},
		{0.0004, -0.0001, 0.0002},
		{0.0009, 0.0002, -0.0004},
		{0.0002, 0.0007, 0.0003},
		{-0.0006, 0.0001, 0.0005},
	};

	static constexpr double HiddenBiasInit[LocalHiddenNeurons] =
	{
		0.01, -0.01, 0.0, 0.005, 0.003, -0.002, 0.004, 0.0, -0.001, 0.002,
	};

	static constexpr double OutputWeightInit[LocalHiddenNeurons] =
	{
		0.02, -0.015, 0.018, 0.017, 0.011, 0.022, 0.013, 0.009, 0.005, 0.006,
	};

	static constexpr double OutputBiasInit[LocalOutputCount] =
	{
		0.1,
	};
}

UNeuralMoonlightEnergyComponent::UNeuralMoonlightEnergyComponent()
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

double UNeuralMoonlightEnergyComponent::Sigmoid(double Value)
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}

double UNeuralMoonlightEnergyComponent::PredictEnergyFlux(double Intensity, double AbsorbingArea, double Distance) const
{
	const double InputValues[InputCount] = { Intensity, AbsorbingArea, Distance };
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

void UNeuralMoonlightEnergyComponent::LogEnergyFluxTest()
{
	constexpr double TestIntensity = 100.0;
	constexpr double TestArea = 10.0;
	constexpr double TestDistance = 384400.0;

	if (UNeuralMoonlightEnergyComponent* TestNetwork = NewObject<UNeuralMoonlightEnergyComponent>(GetTransientPackage()))
	{
		const double EnergyFlux = TestNetwork->PredictEnergyFlux(TestIntensity, TestArea, TestDistance);
		UE_LOG(LogTemp, Log, TEXT("Predicted Energy Flux (Neural): %.4f units"), EnergyFlux);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to instantiate Neural Moonlight Energy component for the test."));
	}
}
