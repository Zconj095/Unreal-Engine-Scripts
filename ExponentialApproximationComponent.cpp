#include "ExponentialApproximationComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

UExponentialApproximationComponent::UExponentialApproximationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UExponentialApproximationComponent::BeginPlay()
{
	Super::BeginPlay();

	LogApproximation();
}

void UExponentialApproximationComponent::LogApproximation() const
{
	const float ExactResult = CalculateExact(P0, Lambda);
	const float Approximation = CalculateApproximation(P0, Lambda);
	const float DeltaP = Approximation - P0;

	UE_LOG(LogTemp, Log, TEXT("Exact Result: %f"), ExactResult);
	UE_LOG(LogTemp, Log, TEXT("Approximated Result: %f"), Approximation);
	UE_LOG(LogTemp, Log, TEXT("Delta P: %f"), DeltaP);
}

float UExponentialApproximationComponent::CalculateExact(float InP0, float InLambda) const
{
	return InP0 * FMath::Exp(InLambda);
}

float UExponentialApproximationComponent::CalculateApproximation(float InP0, float InLambda) const
{
	return InP0 * (1.0f + InLambda);
}
