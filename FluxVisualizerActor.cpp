#include "FluxVisualizerActor.h"

#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"

FMagicalCircuitOptimization::FMagicalCircuitOptimization(double InMaxFlux, double InTolerance)
	: MaxFlux(InMaxFlux)
	, Tolerance(InTolerance)
{
}

std::complex<float> FMagicalCircuitOptimization::ComputeMagicalFlux(double Time, double Frequency) const
{
	const float Phase = static_cast<float>(Frequency * Time);
	const float Real = static_cast<float>(MaxFlux * FMath::Cos(Phase));
	const float Imag = static_cast<float>(MaxFlux * FMath::Sin(Phase) * (1.0f - static_cast<float>(Tolerance)));
	return std::complex<float>(Real, Imag);
}

AFluxVisualizerActor::AFluxVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFluxVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	MagicalCircuit = FMagicalCircuitOptimization(CircuitMaxFlux, CircuitTolerance);

	if (!PointActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no PointActorClass assigned."), *GetName());
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
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 Index = 0; Index < NumPoints; ++Index)
	{
		const double Time = Index * TimeStep;
		const std::complex<float> Flux = MagicalCircuit.ComputeMagicalFlux(Time, Frequency);
		const float FluxMagnitude = std::abs(Flux);
		const float VisualHeight = FMath::Max(FluxMagnitude * HeightScale, KINDA_SMALL_NUMBER);

		const FVector SpawnLocation = BaseLocation + FVector(Index * PointSpacing, 0.0f, VisualHeight);
		AActor* SpawnedPoint = World->SpawnActor<AActor>(PointActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (SpawnedPoint)
		{
			SpawnedPoint->SetActorScale3D(FVector(PointScale, VisualHeight, PointScale));
		}
	}
}
