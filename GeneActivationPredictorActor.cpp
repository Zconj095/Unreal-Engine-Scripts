#include "GeneActivationPredictorActor.h"

#include "Math/UnrealMathUtility.h"
#include "Logging/LogMacros.h"

AGeneActivationPredictorActor::AGeneActivationPredictorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneActivationPredictorActor::BeginPlay()
{
	Super::BeginPlay();
	if (TrainingSamples.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No training samples provided for GeneActivationPredictorActor."));
		return;
	}

	TrainPredictor(1000, 0.01f);
	const double Evaluation = PredictActivationEnergy(TrainingSamples[0].Inputs.IsValidIndex(0) ? TrainingSamples[0].Inputs[0] : 0.0,
	                                                  TrainingSamples[0].Inputs.IsValidIndex(1) ? TrainingSamples[0].Inputs[1] : 0.0);
	UE_LOG(LogTemp, Display, TEXT("Sample prediction after training: %.4f"), Evaluation);
}

void AGeneActivationPredictorActor::TrainPredictor(int32 Epochs, float LearningRate)
{
	if (TrainingSamples.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Training skipped because no samples were configured."));
		return;
	}

	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		double TotalError = 0.0;
		for (const FGeneActivationTrainingSample& Sample : TrainingSamples)
		{
			if (Sample.Inputs.Num() < 2 || Sample.Outputs.Num() == 0)
			{
				continue;
			}

			const double Baseline = Sample.Inputs[0];
			const double Magical = Sample.Inputs[1];
			const double Target = Sample.Outputs[0];

			const double WeightedSum = (Baseline * WeightBaseline) + (Magical * WeightMagical) + Bias;
			const double Prediction = ActivationFunction(WeightedSum);
			const double Error = Target - Prediction;
			TotalError += FMath::Abs(Error);

			const double Derivative = Prediction * (1.0 - Prediction);
			WeightBaseline += LearningRate * Error * Derivative * Baseline;
			WeightMagical += LearningRate * Error * Derivative * Magical;
			Bias += LearningRate * Error * Derivative;
		}

		if (Epoch % 100 == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Epoch %d/%d, Error: %.6f"), Epoch + 1, Epochs, TotalError);
		}
	}
}

double AGeneActivationPredictorActor::PredictActivationEnergy(double BaselineEnergy, double MagicalEnergy) const
{
	const double WeightedSum = (BaselineEnergy * WeightBaseline) + (MagicalEnergy * WeightMagical) + Bias;
	return ActivationFunction(WeightedSum);
}

double AGeneActivationPredictorActor::ActivationFunction(double Value) const
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}
