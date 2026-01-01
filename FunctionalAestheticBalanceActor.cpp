#include "FunctionalAestheticBalanceActor.h"

#include "Math/UnrealMathUtility.h"

AFunctionalAestheticBalanceActor::AFunctionalAestheticBalanceActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFunctionalAestheticBalanceActor::BeginPlay()
{
	Super::BeginPlay();

	const double Elegance = CalculateElegance();
	const double Prediction = PredictEleganceUsingSimpleModel();
	const double TensorFlowValue = bRunTensorFlowLikeCheck ? ComputeTensorFlowLikeValue(Functionality, Aesthetic, Complexity) : Elegance;

	LogResults(Elegance, Prediction, TensorFlowValue);
}

void AFunctionalAestheticBalanceActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bLogEveryTick)
	{
		return;
	}

	const double Elegance = CalculateElegance();
	const double Prediction = PredictEleganceUsingSimpleModel();
	const double TensorFlowValue = bRunTensorFlowLikeCheck ? ComputeTensorFlowLikeValue(Functionality, Aesthetic, Complexity) : Elegance;
	LogResults(Elegance, Prediction, TensorFlowValue);
}

double AFunctionalAestheticBalanceActor::CalculateElegance() const
{
	if (Complexity <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Complexity must be greater than zero when computing elegance."));
		return 0.0;
	}

	return (Functionality * Aesthetic) / Complexity;
}

double AFunctionalAestheticBalanceActor::PredictEleganceUsingSimpleModel() const
{
	// Simple normalized prediction that echoes the manual calculation with a small bias.
	const double NormalizedFunctionality = FMath::Clamp(Functionality / 100.0, 0.0, 1.0);
	const double NormalizedAesthetic = FMath::Clamp(Aesthetic / 100.0, 0.0, 1.0);
	const double BasePrediction = (NormalizedFunctionality * NormalizedAesthetic) * 100.0;
	return BasePrediction + 2.0;
}

double AFunctionalAestheticBalanceActor::ComputeTensorFlowLikeValue(double InFunctionality, double InAesthetic, double InComplexity) const
{
	if (InComplexity <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Complexity must be greater than zero when simulating TensorFlow computation."));
		return 0.0;
	}

	return (InFunctionality * InAesthetic) / InComplexity;
}

void AFunctionalAestheticBalanceActor::LogResults(double EleganceValue, double Prediction, double TensorFlowValue) const
{
	UE_LOG(LogTemp, Display, TEXT("Design Elegance (E): %.4f"), EleganceValue);
	UE_LOG(LogTemp, Display, TEXT("Neural-style Prediction (E): %.4f"), Prediction);
	if (bRunTensorFlowLikeCheck)
	{
		UE_LOG(LogTemp, Display, TEXT("TensorFlow-like Computation (E): %.4f"), TensorFlowValue);
	}
}
