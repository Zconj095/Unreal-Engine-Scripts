#include "EvolutionaryAdaptationTrackingComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

UEvolutionaryAdaptationTrackingComponent::UEvolutionaryAdaptationTrackingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PathwayActivity = 10.0;
	TimeDelta = 0.1;
	MagicalGradient = 5.0;
}

void UEvolutionaryAdaptationTrackingComponent::BeginPlay()
{
	Super::BeginPlay();

	double AdaptiveChangeRate = CalculateAdaptiveChangeRate(PathwayActivity, TimeDelta, MagicalGradient);
	UE_LOG(LogTemp, Log, TEXT("Adaptive Change Rate (ΔB): %f"), AdaptiveChangeRate);
	DrawAdaptationVisualization();
}

float UEvolutionaryAdaptationTrackingComponent::CalculateAdaptiveChangeRate(double Pathway, double Delta, double Gradient) const
{
	if (FMath::IsNearlyZero(Delta))
	{
		UE_LOG(LogTemp, Error, TEXT("Time delta must be non-zero."));
		return 0.0f;
	}

	return static_cast<float>((Pathway / Delta) + Gradient);
}

void UEvolutionaryAdaptationTrackingComponent::DrawAdaptationVisualization() const
{
	if (!GetWorld())
	{
		return;
	}

	for (double Delta = 0.01; Delta <= 1.0; Delta += 0.05)
	{
		const float Rate = CalculateAdaptiveChangeRate(PathwayActivity, Delta, MagicalGradient);
		const FVector Location(Delta * 10.0f, Rate, 0.0f);
		DrawDebugSphere(GetWorld(), Location, 5.0f, 8, FColor::Green, false, 10.0f);
	}
}
