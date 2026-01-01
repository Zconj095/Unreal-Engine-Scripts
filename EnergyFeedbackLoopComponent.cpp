#include "EnergyFeedbackLoopComponent.h"

UEnergyFeedbackLoopComponent::UEnergyFeedbackLoopComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	AdaptationRate = 0.1f;
	InitialFeedbackState = 0.0f;
	ObservedEnergy = 1.0f;
	ExpectedEnergy = 0.5f;
	UpdatedFeedbackState = 0.0f;
	CurrentFeedbackState = 0.0f;
}

void UEnergyFeedbackLoopComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentFeedbackState = InitialFeedbackState;
}

void UEnergyFeedbackLoopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatedFeedbackState = CalculateFeedbackState(CurrentFeedbackState, ObservedEnergy, ExpectedEnergy);
	CurrentFeedbackState = UpdatedFeedbackState;
	LogFeedbackState();
}

float UEnergyFeedbackLoopComponent::CalculateFeedbackState(float CurrentState, float Observed, float Expected) const
{
	return CurrentState + AdaptationRate * (Observed - Expected);
}

void UEnergyFeedbackLoopComponent::LogFeedbackState() const
{
	UE_LOG(LogTemp, Log, TEXT("Observed Energy (O_e): %.2f, Expected Energy (E_e): %.2f"), ObservedEnergy, ExpectedEnergy);
	UE_LOG(LogTemp, Log, TEXT("Current Feedback State (F_n): %.2f, Updated Feedback State (F_n+1): %.2f"), CurrentFeedbackState, UpdatedFeedbackState);
}
