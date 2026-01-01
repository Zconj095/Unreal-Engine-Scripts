#include "RecombinationVisualizerComponent.h"
#include "DrawDebugHelpers.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

URecombinationVisualizerComponent::URecombinationVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URecombinationVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DistanceStep <= 0.0f || DistanceEnd <= DistanceStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid recombination visualization range."));
		return;
	}

	for (float Distance = DistanceStart; Distance <= DistanceEnd; Distance += DistanceStep)
	{
		const float Probability = 1.0f - FMath::Exp(-Beta * Distance);
		Probabilities.Add(Probability);

		const FVector Origin = GetOwner()->GetActorLocation();
		const FVector Position = Origin + FVector(Distance, 0.0f, Probability * HeightScale);
		DrawDebugSphere(GetWorld(), Position, 20.0f, 12, FColor::MakeRandomColor(), true, 0.0f, 0, 2.0f);
		UE_LOG(LogTemp, Log, TEXT("Distance: %.2f, Crossover probability: %.4f"), Distance, Probability);
	}
}
