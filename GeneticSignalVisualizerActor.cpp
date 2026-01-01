#include "GeneticSignalVisualizerActor.h"

#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include <cmath>

constexpr double PI_VALUE = 3.14159265358979323846;

AGeneticSignalVisualizerActor::AGeneticSignalVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticSignalVisualizerActor::BeginPlay()
{
	Super::BeginPlay();
	SpawnSignalPoints();
}

void AGeneticSignalVisualizerActor::SpawnSignalPoints()
{
	if (!PointActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PointActorClass is not assigned."));
		return;
	}

	if (Amplitudes.Num() != Frequencies.Num() || Frequencies.Num() != PhaseShifts.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Signal coefficient arrays must share the same length."));
		return;
	}

	if (EndX <= StartX || Step <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid range or step for signal visualization."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector BaseLocation = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	for (double X = StartX; X <= EndX; X += Step)
	{
		const double Signal = EvaluateSignal(X);
		const FVector SpawnLocation = BaseLocation + FVector(static_cast<float>(X), 0.0f, static_cast<float>(Signal));
		AActor* Spawned = World->SpawnActor<AActor>(PointActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (Spawned)
		{
			Spawned->SetActorScale3D(FVector(PointScale));
		}
		UE_LOG(LogTemp, Display, TEXT("Position: %.2f, Signal: %.4f"), X, Signal);
	}
}

double AGeneticSignalVisualizerActor::EvaluateSignal(double X) const
{
	double Result = 0.0;
	for (int32 Index = 0; Index < Amplitudes.Num(); ++Index)
	{
		Result += Amplitudes[Index] * FMath::Sin(2.0 * PI_VALUE * Frequencies[Index] * X + PhaseShifts[Index]);
	}
	return Result;
}
