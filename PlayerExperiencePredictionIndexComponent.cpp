#include "PlayerExperiencePredictionIndexComponent.h"
#include "Logging/LogMacros.h"

UPlayerExperiencePredictionIndexComponent::UPlayerExperiencePredictionIndexComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerExperiencePredictionIndexComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerExperiencePredictionIndexComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PredictionTime <= 0.0f || CognitiveCost <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Prediction time (T) and cognitive cost (μ) must be greater than zero."));
		PEPI = 0.0f;
		return;
	}

	PEPI = Gamma * (PGame / (PredictionTime * CognitiveCost));
	UE_LOG(LogTemp, Log, TEXT("PEPI: %0.4f"), PEPI);
}

void UPlayerExperiencePredictionIndexComponent::UpdateGamma(float NewGamma)
{
	Gamma = NewGamma;
}

void UPlayerExperiencePredictionIndexComponent::UpdatePGame(float NewPGame)
{
	PGame = NewPGame;
}

void UPlayerExperiencePredictionIndexComponent::UpdatePredictionTime(float NewTime)
{
	if (NewTime > 0.0f)
	{
		PredictionTime = NewTime;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Prediction time (T) must be greater than zero."));
	}
}

void UPlayerExperiencePredictionIndexComponent::UpdateCognitiveCost(float NewCost)
{
	if (NewCost > 0.0f)
	{
		CognitiveCost = NewCost;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cognitive cost (μ) must be greater than zero."));
	}
}
