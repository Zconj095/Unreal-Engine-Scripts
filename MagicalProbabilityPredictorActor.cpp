#include "MagicalProbabilityPredictorActor.h"
#include "Math/UnrealMathUtility.h"

AMagicalProbabilityPredictorActor::AMagicalProbabilityPredictorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMagicalProbabilityPredictorActor::BeginPlay()
{
	Super::BeginPlay();

	LogPrediction();
}

void AMagicalProbabilityPredictorActor::LogPrediction() const
{
	TArray<float> Inputs;
	Inputs.Init(0.15f, InputSize);

	float Sum = 0.f;
	for (float Value : Inputs)
	{
		Sum += Value;
	}

	const float Prediction = FMath::Clamp(Sum / InputSize, 0.f, 1.f);
	UE_LOG(LogTemp, Log, TEXT("Magical probability prediction: %f"), Prediction);
}
