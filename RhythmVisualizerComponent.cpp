#include "RhythmVisualizerComponent.h"
#include "DrawDebugHelpers.h"
#include "Logging/LogMacros.h"

URhythmVisualizerComponent::URhythmVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URhythmVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Points <= 0 || TimeSpan <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid rhythm visualization parameters."));
		return;
	}

	const FVector Origin = GetOwner()->GetActorLocation();
	for (int32 Index = 0; Index <= Points; ++Index)
	{
		const float Time = Index * TimeSpan / Points;
		const float State = Amplitude * FMath::Sin(Frequency * Time + Phase);
		const FVector Position = Origin + FVector(Time * 100.0f, 0.0f, State * 100.0f);
		DrawDebugSphere(GetWorld(), Position, 20.0f, 12, PointColor, true, 0.0f, 0, 2.0f);

		if (bLogValues)
		{
			UE_LOG(LogTemp, Log, TEXT("Time: %.2f hrs, State: %.4f"), Time, State);
		}
	}
}
