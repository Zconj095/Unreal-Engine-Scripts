#include "CoordinalInferentialMeansComponent.h"

#include "GameFramework/Actor.h"

FSpatialPoint::FSpatialPoint() = default;

FSpatialPoint::FSpatialPoint(const FVector& InPosition, float InLikelihood)
	: Position(InPosition)
	, Likelihood(InLikelihood)
{
}

UCoordinalInferentialMeansComponent::UCoordinalInferentialMeansComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCoordinalInferentialMeansComponent::BeginPlay()
{
	Super::BeginPlay();

	// Provide default data so the component behaves like the Unity example out of the box.
	if (SpatialPoints.Num() == 0)
	{
		SpatialPoints = {
			FSpatialPoint(FVector(1.f, 2.f, 3.f), 0.3f),
			FSpatialPoint(FVector(5.f, 1.f, 4.f), 0.8f),
			FSpatialPoint(FVector(10.f, 0.f, 0.f), 0.2f)};
	}
}

void UCoordinalInferentialMeansComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FVector PlayerPosition = Owner->GetActorLocation();
	ProcessPoints(PlayerPosition);
}

void UCoordinalInferentialMeansComponent::ProcessPoints(const FVector& PlayerPosition)
{
	for (const FSpatialPoint& Point : SpatialPoints)
	{
		const float Distance = FVector::Dist(PlayerPosition, Point.Position);

		if (Distance <= MaxDistance)
		{
			const float InferredLikelihood = InferLikelihood(Distance, Point.Likelihood);
			UE_LOG(LogTemp, Log, TEXT("Inferred likelihood for point at %s is: %.3f"), *Point.Position.ToString(), InferredLikelihood);

			if (InferredLikelihood >= LikelihoodThreshold)
			{
				UE_LOG(LogTemp, Warning, TEXT("High likelihood event detected at point %s!"), *Point.Position.ToString());
				// User hook: trigger custom gameplay logic here (events, delegates, etc.).
			}
		}
	}
}

float UCoordinalInferentialMeansComponent::InferLikelihood(float Distance, float BaseLikelihood) const
{
	const float DistanceFactor = FMath::Max(0.f, 1.f - (Distance / MaxDistance));
	const float InferredLikelihood = BaseLikelihood * DistanceFactor;
	return FMath::Clamp(InferredLikelihood, 0.f, 1.f);
}
