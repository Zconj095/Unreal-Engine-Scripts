#include "GlyphProbabilityPredictorActor.h"

#include "Math/UnrealMathUtility.h"

AGlyphProbabilityPredictorActor::AGlyphProbabilityPredictorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGlyphProbabilityPredictorActor::BeginPlay()
{
	Super::BeginPlay();

	InitializeWeights();
	if (bAutoTrain)
	{
		TrainPredictor(Epochs, LearningRate);
	}
}

void AGlyphProbabilityPredictorActor::InitializeWeights()
{
	WeightMatrix.SetNum(OutputSize);
	for (int32 Out = 0; Out < OutputSize; ++Out)
	{
		WeightMatrix[Out].SetNum(InputSize);
		for (int32 In = 0; In < InputSize; ++In)
		{
			WeightMatrix[Out][In] = FMath::FRandRange(-0.5, 0.5);
		}
	}
	Biases.Init(0.5, OutputSize);
}

void AGlyphProbabilityPredictorActor::TrainPredictor(int32 InEpochs, float InLearningRate)
{
	if (TrainingSamples.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No training samples provided."));
		return;
	}

	for (int32 Epoch = 0; Epoch < InEpochs; ++Epoch)
	{
		double TotalError = 0.0;
		for (const FGlyphProbabilitySample& Sample : TrainingSamples)
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

				double Prediction = ActivationFunction(WeightedSum);
				double Error = Sample.Targets[OutputIndex] - Prediction;
				double Derivative = Prediction * (1.0 - Prediction);
				TotalError += FMath::Abs(Error);

				for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
				{
					WeightMatrix[OutputIndex][InputIndex] += InLearningRate * Error * Derivative * Sample.Inputs[InputIndex];
				}
				Biases[OutputIndex] += InLearningRate * Error * Derivative;
			}
		}

		if (Epoch % 10 == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Epoch %d/%d, Error: %.6f"), Epoch + 1, InEpochs, TotalError);
		}
	}
}

TArray<double> AGlyphProbabilityPredictorActor::Predict(const TArray<double>& Inputs) const
{
	TArray<double> Output;
	if (Inputs.Num() != InputSize || OutputSize == 0)
	{
		return Output;
	}

	Output.Init(0.0, OutputSize);
	for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
	{
		double WeightedSum = 0.0;
		for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
		{
			WeightedSum += Inputs[InputIndex] * WeightMatrix[OutputIndex][InputIndex];
		}
		WeightedSum += Biases[OutputIndex];
		Output[OutputIndex] = ActivationFunction(WeightedSum);
	}
	return Output;
}

double AGlyphProbabilityPredictorActor::ActivationFunction(double Value) const
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}
