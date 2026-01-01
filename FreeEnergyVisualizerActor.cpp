#include "FreeEnergyVisualizerActor.h"

#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"

FThermodynamicModel::FThermodynamicModel(double InMagicalEnhancementFactor)
	: MagicalEnhancementFactor(InMagicalEnhancementFactor)
{
}

TArray<double> FThermodynamicModel::SimulateFreeEnergyChanges(const TArray<double>& EnthalpyChanges,
                                                              const TArray<double>& Temperatures,
                                                              const TArray<double>& EntropyChanges,
                                                              const TArray<double>& MagicalEnergies) const
{
	const int32 NumEntries = FMath::Min(
		FMath::Min(EnthalpyChanges.Num(), Temperatures.Num()),
		FMath::Min(EntropyChanges.Num(), MagicalEnergies.Num()));
	TArray<double> Result;
	Result.Reserve(NumEntries);

	for (int32 Index = 0; Index < NumEntries; ++Index)
	{
		const double DeltaG = EnthalpyChanges[Index] - (Temperatures[Index] * EntropyChanges[Index]);
		const double MagicalContribution = MagicalEnergies[Index] * MagicalEnhancementFactor;
		Result.Add(DeltaG + MagicalContribution);
	}

	if (Result.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Free energy simulation produced no entries; ensure input arrays share the same length."));
	}

	return Result;
}

AFreeEnergyVisualizerActor::AFreeEnergyVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFreeEnergyVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (MagicalEnhancementFactor <= 0.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Magical enhancement factor should be positive to produce meaningful free energy values."));
	}

	FThermodynamicModel Model(MagicalEnhancementFactor);
	TArray<double> FreeEnergyChanges = Model.SimulateFreeEnergyChanges(EnthalpyChanges, Temperatures, EntropyChanges, MagicalEnergies);
	SpawnVisualizationPoints(FreeEnergyChanges);
}

void AFreeEnergyVisualizerActor::SpawnVisualizationPoints(const TArray<double>& FreeEnergyChanges)
{
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

	FVector BaseLocation = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 Index = 0; Index < FreeEnergyChanges.Num(); ++Index)
	{
		const float Height = static_cast<float>(FreeEnergyChanges[Index] * HeightScale);
		const FVector SpawnLocation = BaseLocation + FVector(Index * PointSpacing, 0.0f, Height);
		AActor* SpawnedPoint = World->SpawnActor<AActor>(PointActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (SpawnedPoint)
		{
			const float SizeZ = FMath::Max(FMath::Abs(Height) * 0.1f, KINDA_SMALL_NUMBER);
			SpawnedPoint->SetActorScale3D(FVector(SizeScale, SizeScale, SizeZ));
		}
	}
}
