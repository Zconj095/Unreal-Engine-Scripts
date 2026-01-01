#include "GeneticSignalPredictorActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticSignalPredictorActor::AGeneticSignalPredictorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticSignalPredictorActor::BeginPlay()
{
	Super::BeginPlay();

	if (TrainingSamples.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No training samples configured."));
		return;
	}

	const FGeneticSignalTrainingSample& ReferenceSample = TrainingSamples[0];
	InputCount = ReferenceSample.Inputs.Num();
	OutputCount = ReferenceSample.Outputs.Num();

	if (InputCount == 0 || OutputCount == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Training samples must contain inputs and outputs."));
		return;
	}

	InitializeWeights(InputCount, OutputCount);

	if (bAutoTrain)
	{
		TrainPredictor(Epochs, LearningRate);
	}

	TArray<double> Prediction = PredictSignal(ReferenceSample.Inputs);
	FString Joined;
	for (double Value : Prediction)
	{
		Joined += FString::Printf(TEXT("%.4f "), Value);
	}
	UE_LOG(LogTemp, Display, TEXT("Sample prediction after training: %s"), *Joined);
}

void AGeneticSignalPredictorActor::InitializeWeights(int32 InInputCount, int32 InOutputCount)
{
	InputCount = InInputCount;
	OutputCount = InOutputCount;

	WeightMatrix.SetNum(OutputCount);
	for (int32 Index = 0; Index < OutputCount; ++Index)
	{
		WeightMatrix[Index].SetNum(InputCount);
		for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
		{
			WeightMatrix[Index][InputIndex] = FMath::FRandRange(-0.5, 0.5);
		}
	}

	Biases.Init(0.5, OutputCount);
}

void AGeneticSignalPredictorActor::TrainPredictor(int32 InEpochs, float InLearningRate)
{
	if (InputCount == 0 || OutputCount == 0)
	{
		return;
	}

	for (int32 Epoch = 0; Epoch < InEpochs; ++Epoch)
	{
		double TotalError = 0.0;

		for (const FGeneticSignalTrainingSample& Sample : TrainingSamples)
		{
			if (Sample.Inputs.Num() != InputCount || Sample.Outputs.Num() != OutputCount)
			{
				continue;
			}

			for (int32 OutputIndex = 0; OutputIndex < OutputCount; ++OutputIndex)
			{
				double WeightedSum = 0.0;
				for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
				{
					WeightedSum += Sample.Inputs[InputIndex] * WeightMatrix[OutputIndex][InputIndex];
				}
				WeightedSum += Biases[OutputIndex];

				const double Prediction = ActivationFunction(WeightedSum);
				const double Error = Sample.Outputs[OutputIndex] - Prediction;
				const double Derivative = Prediction * (1.0 - Prediction);
				TotalError += FMath::Abs(Error);

				for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
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

TArray<double> AGeneticSignalPredictorActor::PredictSignal(const TArray<double>& Inputs) const
{
	TArray<double> Predictions;
	if (Inputs.Num() != InputCount || OutputCount == 0)
	{
		return Predictions;
	}

	Predictions.Init(0.0, OutputCount);
	for (int32 OutputIndex = 0; OutputIndex < OutputCount; ++OutputIndex)
	{
		double WeightedSum = 0.0;
		for (int32 InputIndex = 0; InputIndex < InputCount; ++InputIndex)
		{
			WeightedSum += Inputs[InputIndex] * WeightMatrix[OutputIndex][InputIndex];
		}
		WeightedSum += Biases[OutputIndex];
		Predictions[OutputIndex] = ActivationFunction(WeightedSum);
	}
	return Predictions;
}

double AGeneticSignalPredictorActor::ActivationFunction(double Value) const
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}
