#include "NeuralStructuralAnalysisComponent.h"

#include "Math/UnrealMathUtility.h"

namespace
{
	static constexpr int32 LocalInputCount = 2;
	static constexpr int32 LocalHiddenNeurons = 5;

	static constexpr double HiddenWeightInit[LocalHiddenNeurons][LocalInputCount] =
	{
		{0.015, -0.018},
		{-0.011, 0.022},
		{0.019, -0.007},
		{-0.013, 0.017},
		{0.006, 0.012},
	};

	static constexpr double HiddenBiasInit[LocalHiddenNeurons] =
	{
		0.02, -0.01, 0.015, -0.005, 0.01,
	};

	static constexpr double OutputWeightInit[LocalHiddenNeurons] =
	{
		0.023, -0.019, 0.017, 0.01, -0.01,
	};

	static constexpr double OutputBiasInit[1] =
	{
		0.0,
	};
}

UNeuralStructuralAnalysisComponent::UNeuralStructuralAnalysisComponent()
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

double UNeuralStructuralAnalysisComponent::Sigmoid(double Value)
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}

double UNeuralStructuralAnalysisComponent::SigmoidDerivative(double SigmoidValue)
{
	return SigmoidValue * (1.0 - SigmoidValue);
}

void UNeuralStructuralAnalysisComponent::TrainModel(const TArray<FVector2D>& Inputs, const TArray<double>& Targets, int32 Epochs, double LearningRate)
{
	if (Inputs.Num() == 0 || Inputs.Num() != Targets.Num() || Epochs <= 0 || LearningRate <= 0.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid training parameters for structural model."));
		return;
	}

	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		double TotalError = 0.0;

		for (int32 SampleIndex = 0; SampleIndex < Inputs.Num(); ++SampleIndex)
		{
			const double InputValues[InputCount] = { Inputs[SampleIndex].X, Inputs[SampleIndex].Y };
			double HiddenOutputs[HiddenNeurons];

			for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
			{
				double Sum = HiddenBiases[HiddenIndex];
				const int32 RowStart = HiddenIndex * InputCount;

				for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
				{
					Sum += HiddenWeights[RowStart + InputIndex] * InputValues[InputIndex];
				}

				HiddenOutputs[HiddenIndex] = Sigmoid(Sum);
			}

			double OutputSum = OutputBiases[0];
			for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
			{
				OutputSum += OutputWeights[HiddenIndex] * HiddenOutputs[HiddenIndex];
			}

			const double OutputValue = Sigmoid(OutputSum);
			const double TargetValue = Targets[SampleIndex];
			const double SampleError = TargetValue - OutputValue;
			TotalError += SampleError * SampleError;

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
			const double MSE = TotalError / Inputs.Num();
			UE_LOG(LogTemp, Log, TEXT("Structural Epoch %d/%d - MSE: %.6f"), Epoch + 1, Epochs, MSE);
		}
	}
}

double UNeuralStructuralAnalysisComponent::PredictStress(double Force, double Area) const
{
	const double InputValues[InputCount] = { Force, Area };
	double HiddenOutputs[HiddenNeurons];

	for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
	{
		double Sum = HiddenBiases[HiddenIndex];
		const int32 RowStart = HiddenIndex * InputCount;

		for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
		{
			Sum += HiddenWeights[RowStart + InputIndex] * InputValues[InputIndex];
		}

		HiddenOutputs[HiddenIndex] = Sigmoid(Sum);
	}

	double OutputSum = OutputBiases[0];
	for (int32 HiddenIndex = 0; HiddenIndex < HiddenNeurons; ++HiddenIndex)
	{
		OutputSum += OutputWeights[HiddenIndex] * HiddenOutputs[HiddenIndex];
	}

	return Sigmoid(OutputSum);
}

void UNeuralStructuralAnalysisComponent::LogStructuralTest()
{
	TArray<FVector2D> Inputs;
	TArray<double> Targets;

	Inputs.Add(FVector2D(1000.0, 50.0));
	Inputs.Add(FVector2D(2000.0, 100.0));
	Inputs.Add(FVector2D(500.0, 25.0));

	Targets.Add(20.0);
	Targets.Add(20.0);
	Targets.Add(20.0);

	TrainModel(Inputs, Targets, 1000, 0.01);

	const double Prediction = PredictStress(1500.0, 75.0);
	UE_LOG(LogTemp, Log, TEXT("Predicted Stress: %.4f Pa"), Prediction);
}
