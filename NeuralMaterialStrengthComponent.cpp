#include "NeuralMaterialStrengthComponent.h"

#include "Math/UnrealMathUtility.h"

namespace
{
	static constexpr int32 LocalInputCount = 2;
	static constexpr int32 LocalHiddenNeurons = 5;

	static constexpr double HiddenWeightInit[LocalHiddenNeurons][LocalInputCount] =
	{
		{0.012, -0.017},
		{-0.023, 0.019},
		{0.031, -0.008},
		{-0.014, 0.026},
		{0.009, 0.011},
	};

	static constexpr double HiddenBiasInit[LocalHiddenNeurons] =
	{
		0.01, -0.01, 0.015, -0.005, 0.007,
	};

	static constexpr double OutputWeightInit[LocalHiddenNeurons] =
	{
		0.02, -0.015, 0.017, 0.01, -0.008,
	};

	static constexpr double OutputBiasInit[1] =
	{
		0.0,
	};
}

UNeuralMaterialStrengthComponent::UNeuralMaterialStrengthComponent()
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

double UNeuralMaterialStrengthComponent::Sigmoid(double Value)
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}

double UNeuralMaterialStrengthComponent::SigmoidDerivative(double SigmoidValue)
{
	return SigmoidValue * (1.0 - SigmoidValue);
}

void UNeuralMaterialStrengthComponent::TrainModel(const TArray<FVector2D>& Inputs, const TArray<double>& Targets, int32 Epochs, double LearningRate)
{
	if (Inputs.Num() == 0 || Inputs.Num() != Targets.Num() || Epochs <= 0 || LearningRate <= 0.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Training data is invalid."));
		return;
	}

	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		double EpochError = 0.0;

		for (int32 SampleIndex = 0; SampleIndex < Inputs.Num(); ++SampleIndex)
		{
			const double InputValues[InputCount] = { Inputs[SampleIndex].X, Inputs[SampleIndex].Y };
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

			double OutputAccumulator = OutputBiases[0];
			for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
			{
				OutputAccumulator += OutputWeights[HiddenIndex] * HiddenOutputs[HiddenIndex];
			}

			const double OutputValue = Sigmoid(OutputAccumulator);
			const double TargetValue = Targets[SampleIndex];
			const double SampleError = TargetValue - OutputValue;
			EpochError += SampleError * SampleError;

			const double OutputDelta = SampleError * SigmoidDerivative(OutputValue);
			OutputBiases[0] += LearningRate * OutputDelta;

			for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
			{
				const double HiddenDelta = OutputWeights[HiddenIndex] * OutputDelta * SigmoidDerivative(HiddenOutputs[HiddenIndex]);

				for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
				{
					HiddenWeights[HiddenIndex * InputCount + InputIndex] += LearningRate * HiddenDelta * InputValues[InputIndex];
				}

				HiddenBiases[HiddenIndex] += LearningRate * HiddenDelta;
				OutputWeights[HiddenIndex] += LearningRate * OutputDelta * HiddenOutputs[HiddenIndex];
			}
		}

		if (Epochs <= 20 || Epoch % 50 == 0 || Epoch == Epochs - 1)
		{
			const double MeanSquaredError = EpochError / Inputs.Num();
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d - MSE: %.6f"), Epoch + 1, Epochs, MeanSquaredError);
		}
	}
}

double UNeuralMaterialStrengthComponent::PredictStrain(double Stress, double YoungsModulus) const
{
	const double InputValues[InputCount] = { Stress, YoungsModulus };
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

	double OutputAccumulator = OutputBiases[0];
	for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
	{
		OutputAccumulator += OutputWeights[HiddenIndex] * HiddenOutputs[HiddenIndex];
	}

	return Sigmoid(OutputAccumulator);
}

void UNeuralMaterialStrengthComponent::LogTrainingTest()
{
	TArray<FVector2D> Inputs;
	TArray<double> Targets;

	Inputs.Add(FVector2D(500.0, 200000.0));
	Inputs.Add(FVector2D(600.0, 250000.0));
	Inputs.Add(FVector2D(400.0, 180000.0));

	Targets.Add(0.0025);
	Targets.Add(0.0024);
	Targets.Add(0.00222);

	TrainModel(Inputs, Targets, 1000, 0.01);

	const double Prediction = PredictStrain(700.0, 300000.0);
	UE_LOG(LogTemp, Log, TEXT("Predicted Strain: %.8f"), Prediction);
}
