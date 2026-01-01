#include "CrossValidatorComponent.h"

#include "Logging/LogMacros.h"

UCrossValidatorComponent::UCrossValidatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Simple default data
	DataPoints = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
	K = 3;
}

void UCrossValidatorComponent::BeginPlay()
{
	Super::BeginPlay();

	const bool bHasDelegate = ModelTrainer.IsBound();
	if (!bHasDelegate)
	{
		// Provide a dummy trainer that returns random accuracy for the demo.
		FModelTrainerDelegate Dummy;
		Dummy.BindUFunction(this, FName("GenerateRandomAccuracy"));
		const float AvgAcc = KFoldCrossValidation(DataPoints, K, Dummy);
		UE_LOG(LogTemp, Log, TEXT("Cross-validation (dummy trainer) average accuracy over %d folds: %.3f"), K, AvgAcc);
	}
	else
	{
		const float AvgAcc = KFoldCrossValidation(DataPoints, K, ModelTrainer);
		UE_LOG(LogTemp, Log, TEXT("Cross-validation average accuracy over %d folds: %.3f"), K, AvgAcc);
	}
}

float UCrossValidatorComponent::GenerateRandomAccuracy(const TArray<float>& /*TrainingSet*/)
{
	return FMath::FRandRange(0.5f, 1.0f);
}

float UCrossValidatorComponent::KFoldCrossValidation(const TArray<float>& Dataset, int32 Folds, const FModelTrainerDelegate& Trainer) const
{
	if (Folds <= 1)
	{
		UE_LOG(LogTemp, Error, TEXT("KFoldCrossValidation: Folds must be greater than 1."));
		return 0.f;
	}
	if (Dataset.Num() < Folds)
	{
		UE_LOG(LogTemp, Error, TEXT("KFoldCrossValidation: Not enough data points (%d) for %d folds."), Dataset.Num(), Folds);
		return 0.f;
	}
	if (!Trainer.IsBound())
	{
		UE_LOG(LogTemp, Error, TEXT("KFoldCrossValidation: Trainer delegate is not bound."));
		return 0.f;
	}

	TArray<TArray<float>> FoldsData;
	CreateKFolds(Dataset, Folds, FoldsData);

	double TotalAccuracy = 0.0;

	for (int32 i = 0; i < Folds; ++i)
	{
		TArray<float> TrainingSet;
		TArray<float> ValidationSet = FoldsData[i];

		for (int32 j = 0; j < Folds; ++j)
		{
			if (i != j)
			{
				TrainingSet.Append(FoldsData[j]);
			}
		}

		const float Accuracy = Trainer.Execute(TrainingSet);
		TotalAccuracy += Accuracy;
	}

	return static_cast<float>(TotalAccuracy / Folds);
}

void UCrossValidatorComponent::CreateKFolds(const TArray<float>& Dataset, int32 Folds, TArray<TArray<float>>& OutFolds) const
{
	OutFolds.SetNum(Folds);

	// Copy and shuffle
	TArray<float> Shuffled = Dataset;
	for (int32 i = Shuffled.Num() - 1; i > 0; --i)
	{
		const int32 SwapIdx = FMath::RandRange(0, i);
		Shuffled.Swap(i, SwapIdx);
	}

	for (int32 i = 0; i < Shuffled.Num(); ++i)
	{
		const int32 FoldIndex = i % Folds;
		OutFolds[FoldIndex].Add(Shuffled[i]);
	}
}
