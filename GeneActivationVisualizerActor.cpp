#include "GeneActivationVisualizerActor.h"

#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"

AGeneActivationVisualizerActor::AGeneActivationVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneActivationVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	SpawnVisualizationPoints();
}

void AGeneActivationVisualizerActor::SpawnVisualizationPoints()
{
	if (!PointActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GeneActivationVisualizerActor requires a PointActorClass."));
		return;
	}

	if (MagicalEnergyEnd < MagicalEnergyStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("MagicalEnergyEnd must be greater than or equal to MagicalEnergyStart."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FVector BaseLocation = GetActorLocation() + VisualizationOffset;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (double MagicalEnergy = MagicalEnergyStart; MagicalEnergy <= MagicalEnergyEnd; MagicalEnergy += MagicalEnergyStep)
	{
		double ActivationEnergy = CalculateActivationEnergy(MagicalEnergy);
		const float X = static_cast<float>(MagicalEnergy);
		const float Z = static_cast<float>(ActivationEnergy);

		FVector SpawnLocation = BaseLocation + FVector(X, 0.0f, Z);
		AActor* SpawnedPoint = World->SpawnActor<AActor>(PointActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		if (SpawnedPoint)
		{
			SpawnedPoint->SetActorScale3D(FVector(PointScale));
		}

		UE_LOG(LogTemp, Display, TEXT("Magical Energy: %.2f, Activation Energy: %.4f"), MagicalEnergy, ActivationEnergy);
	}
}

double AGeneActivationVisualizerActor::CalculateActivationEnergy(double MagicalEnergy) const
{
	return BaselineEnergy + MagicalEnergy;
}
