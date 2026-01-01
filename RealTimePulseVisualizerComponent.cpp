// Copyright Epic Games, Inc.

#include "RealTimePulseVisualizerComponent.h"

#include "Engine/World.h"

URealTimePulseVisualizerComponent::URealTimePulseVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URealTimePulseVisualizerComponent::PlayPulse(double Amplitude, double Frequency, double Duration)
{
	if (!GetWorld())
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* Pulse = GetWorld()->SpawnActor<AActor>(PulseActorClass ? *PulseActorClass : AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (!Pulse)
	{
		return;
	}

	Pulse->SetActorScale3D(FVector(static_cast<float>(Amplitude), static_cast<float>(Frequency), static_cast<float>(Duration)));

	FPulseInstance Instance;
	Instance.Actor = Pulse;
	Instance.RemainingTime = FMath::Max(0.f, static_cast<float>(Duration));
	ActivePulses.Add(Instance);
}

void URealTimePulseVisualizerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 Index = ActivePulses.Num() - 1; Index >= 0; --Index)
	{
		FPulseInstance& Instance = ActivePulses[Index];
		if (!Instance.Actor.IsValid())
		{
			ActivePulses.RemoveAt(Index);
			continue;
		}

		Instance.RemainingTime -= DeltaTime;
		if (Instance.RemainingTime <= 0.f)
		{
			Instance.Actor->Destroy();
			ActivePulses.RemoveAt(Index);
			continue;
		}

		// Slight growth effect each tick
		AActor* PulseActor = Instance.Actor.Get();
		const FVector NewScale = PulseActor->GetActorScale3D() * 1.01f;
		PulseActor->SetActorScale3D(NewScale);
	}
}
