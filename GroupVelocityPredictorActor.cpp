#include "GroupVelocityPredictorActor.h"

#include "Math/UnrealMathUtility.h"

AGroupVelocityPredictorActor::AGroupVelocityPredictorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGroupVelocityPredictorActor::BeginPlay()
{
	Super::BeginPlay();
	InitializeWeights();
	if (bAutoTrain)
	{
		TrainPredictor(Epochs, LearningRate);
	}
}

void AGroupVelocityPredictorActor::InitializeWeights()
{
	WeightMatrix.SetNum(OutputSize);
	for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
	{
		WeightMatrix[OutputIndex].SetNum(InputSize);
		for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
		{
			WeightMatrix[OutputIndex][InputIndex] = FMath::FRandRange(-0.5, 0.5);
		}
	}
	Biases.Init(0.5, OutputSize);
}

void AGroupVelocityPredictorActor::TrainPredictor(int32 InEpochs, float InLearningRate)
{
	if (TrainingSamples.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No training samples configured."));
		return;
	}

	for (int32 Epoch = 0; Epoch < InEpochs; ++Epoch)
	{
		double TotalError = 0.0;
		for (const FGroupVelocitySample& Sample : TrainingSamples)
		{
			if (Sample.Inputs.Num() != InputSize || Sample.Targets.Num() != OutputSize)
			{
				continue;
			}

			for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
			{
				double WeightedSum = 0.0;
				for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
				{
					WeightedSum += Sample.Inputs[InputIndex] * WeightMatrix[OutputIndex][InputIndex];
				}
				WeightedSum += Biases[OutputIndex];

				const double Prediction = ActivationFunction(WeightedSum);
				const double Error = Sample.Targets[OutputIndex] - Prediction;
				const double Derivative = Prediction * (1.0 - Prediction);
				TotalError += FMath::Abs(Error);

				for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
				{
					WeightMatrix[OutputIndex][InputIndex] += InLearningRate * Error * Derivative * Sample.Inputs[InputIndex];
				}

				Biases[OutputIndex] += InLearningRate * Error * Derivative;
			}
		}

		if (Epoch % 100 == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Epoch %d/%d, Error: %.6f"), Epoch + 1, InEpochs, TotalError);
		}
	}
}

TArray<double> AGroupVelocityPredictorActor::Predict(const TArray<double>& Inputs) const
{
	TArray<double> Outputs;
	if (Inputs.Num() != InputSize || OutputSize == 0)
	{
		return Outputs;
	}

	Outputs.Init(0.0, OutputSize);
	for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
	{
		double WeightedSum = 0.0;
		for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
		{
			WeightedSum += Inputs[InputIndex] * WeightMatrix[OutputIndex][InputIndex];
		}
		WeightedSum += Biases[OutputIndex];
		Outputs[OutputIndex] = ActivationFunction(WeightedSum);
	}
	return Outputs;
}

double AGroupVelocityPredictorActor::ActivationFunction(double Value) const
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}
