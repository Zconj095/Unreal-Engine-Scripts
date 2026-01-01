#include "EnergyVisualizerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnergyTransferModelComponent.h"

UEnergyVisualizerComponent::UEnergyVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MagicalEnhancement = 2.0;
	DeltaHs = { -50.0, -40.0, -30.0 };
	Temperatures = { 300.0, 310.0, 320.0 };
	DeltaSs = { 0.1, 0.2, 0.3 };
	MagicalEnergies = { 10.0, 15.0, 20.0 };
}

void UEnergyVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
	Visualize();
}

void UEnergyVisualizerComponent::Visualize()
{
	if (!PointActorClass || !GetWorld())
	{
		return;
	}

	for (int32 Index = 0; Index < DeltaHs.Num(); ++Index)
	{
		if (Index >= Temperatures.Num() || Index >= DeltaSs.Num() || Index >= MagicalEnergies.Num())
		{
			break;
		}

		double Height = DeltaHs[Index] - (Temperatures[Index] * DeltaSs[Index]) - (MagicalEnhancement * MagicalEnergies[Index]);
		const FVector Location(Index, Height, 0.0f);
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(PointActorClass, Location, FRotator::ZeroRotator);
		if (Spawned)
		{
			const float Scale = FMath::Abs(Height) / 10.0f;
			Spawned->SetActorScale3D(FVector(0.5f, Scale, 0.5f));
		}
	}
}
