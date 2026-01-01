#include "InferentialStatisticsComponent.h"

#include "Logging/LogMacros.h"

UInferentialStatisticsComponent::UInferentialStatisticsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInferentialStatisticsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataSet.Num() == 0)
	{
		// Example data for demonstration
		const float DemoValues[] = {10.f, 12.f, 14.f, 11.f, 13.f, 15.f, 10.f, 14.f, 12.f, 13.f};
		DataSet.Append(DemoValues, UE_ARRAY_COUNT(DemoValues));
	}

	CalculateStatistics();
	MakePrediction();
	MakePredictionWithConfidenceInterval(0.95f);
}

void UInferentialStatisticsComponent::CalculateStatistics()
{
	if (DataSet.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dataset is empty. Cannot calculate statistics."));
		return;
	}

	const float Mean = CalculateMean(DataSet);
	const float StdDev = CalculateStandardDeviation(DataSet, Mean);

	UE_LOG(LogTemp, Log, TEXT("Mean: %.2f"), Mean);
	UE_LOG(LogTemp, Log, TEXT("Standard Deviation: %.2f"), StdDev);
}

float UInferentialStatisticsComponent::CalculateMean(const TArray<float>& Data) const
{
	if (Data.Num() == 0)
	{
		return 0.f;
	}

	double Sum = 0.0;
	for (float V : Data)
	{
		Sum += V;
	}
	return static_cast<float>(Sum / Data.Num());
}

float UInferentialStatisticsComponent::CalculateStandardDeviation(const TArray<float>& Data, float Mean) const
{
	if (Data.Num() == 0)
	{
		return 0.f;
	}

	double SumSq = 0.0;
	for (float V : Data)
	{
		const double Diff = V - Mean;
		SumSq += Diff * Diff;
	}
	return static_cast<float>(FMath::Sqrt(SumSq / Data.Num()));
}

void UInferentialStatisticsComponent::MakePrediction()
{
	if (DataSet.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough data to make a prediction."));
		return;
	}

	const float Mean = CalculateMean(DataSet);
	const float StdDev = CalculateStandardDeviation(DataSet, Mean);

	// Generate prediction with small random deviation
	PredictedValue = Mean + FMath::FRandRange(-StdDev, StdDev);

	UE_LOG(LogTemp, Log, TEXT("Predicted Value: %.2f"), PredictedValue);
}

void UInferentialStatisticsComponent::MakePredictionWithConfidenceInterval(float ConfidenceLevel)
{
	if (DataSet.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough data to calculate a confidence interval."));
		return;
	}

	const float Mean = CalculateMean(DataSet);
	const float StdDev = CalculateStandardDeviation(DataSet, Mean);
	const float Z = GetZScoreForConfidenceLevel(ConfidenceLevel);

	const float Margin = Z * (StdDev / FMath::Sqrt(static_cast<float>(DataSet.Num())));
	const float Lower = Mean - Margin;
	const float Upper = Mean + Margin;

	UE_LOG(LogTemp, Log, TEXT("Predicted Range with %.0f%% confidence: [%.2f, %.2f]"), ConfidenceLevel * 100.f, Lower, Upper);
}

float UInferentialStatisticsComponent::GetZScoreForConfidenceLevel(float ConfidenceLevel) const
{
	if (FMath::IsNearlyEqual(ConfidenceLevel, 0.95f))
	{
		return 1.96f;
	}
	if (FMath::IsNearlyEqual(ConfidenceLevel, 0.90f))
	{
		return 1.64f;
	}
	return 1.96f; // default to 95%
}
