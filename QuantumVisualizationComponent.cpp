#include "QuantumVisualizationComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UQuantumVisualizationComponent::UQuantumVisualizationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuantumVisualizationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GridPoints <= 0 || TimeStep <= 0.0f || GridSpacing <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid quantum visualization parameters."));
		return;
	}

	WavefunctionNorm = 0.0f; // Will be updated in Tick
}

void UQuantumVisualizationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<FVector2D> RealWave;
	RealWave.SetNumZeroed(GridPoints);
	TArray<float> Potential;
	Potential.SetNumZeroed(GridPoints);

	for (int32 Index = 0; Index < GridPoints; ++Index)
	{
		const float X = Index * GridSpacing - GridPoints * GridSpacing / 2.0f;
		RealWave[Index].X = FMath::Exp(-X * X);
		RealWave[Index].Y = 0.0f;
		Potential[Index] = FMath::Exp(-X * X / (2.0f * PotentialWidth * PotentialWidth)) * MagicalPotentialStrength;
	}

	WavefunctionNorm = 0.0f;
	for (const FVector2D& Value : RealWave)
	{
		WavefunctionNorm += Value.X * Value.X + Value.Y * Value.Y;
	}

	WavefunctionNorm *= GridSpacing;
	UE_LOG(LogTemp, Log, TEXT("Wavefunction Norm: %0.4f"), WavefunctionNorm);
}
