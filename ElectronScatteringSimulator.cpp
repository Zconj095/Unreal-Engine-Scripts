#include "ElectronScatteringSimulator.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

AElectronScatteringSimulator::AElectronScatteringSimulator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AElectronScatteringSimulator::BeginPlay()
{
	Super::BeginPlay();

	InitializeElectrons();
}

void AElectronScatteringSimulator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Electrons.Num() == 0)
	{
		return;
	}

	SimulateScattering();

	for (const FVector& Position : FinalPositions)
	{
		DrawDebugSphere(GetWorld(), Position, 2.0f, 12, FColor::Red, false, -1.0f);
	}
}

void AElectronScatteringSimulator::InitializeElectrons()
{
	Electrons.Reset();
	FinalPositions.Reset();

	Electrons.Reserve(ElectronCount);
	FinalPositions.Reserve(ElectronCount / 10);

	for (int32 Index = 0; Index < ElectronCount; ++Index)
	{
		Electrons.Emplace(
			GetActorLocation(),
			GetActorForwardVector(),
			InitialEnergy,
			MaxScatteringAngle
		);
	}

	UE_LOG(LogTemp, Log, TEXT("ElectronScatteringSimulator: Initialized %d electrons."), ElectronCount);
}

void AElectronScatteringSimulator::SimulateScattering()
{
	if (!GetWorld())
	{
		return;
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	for (int32 Index = Electrons.Num() - 1; Index >= 0; --Index)
	{
		FScatteredElectron& Electron = Electrons[Index];
		Electron.Move(StepDistance);

		const FVector TraceStart = Electron.Position;
		const FVector TraceEnd = TraceStart + Electron.Direction * StepDistance;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, static_cast<ECollisionChannel>(SampleChannel.GetValue()), QueryParams))
		{
			Electron.Scatter(Hit.Normal);
			Electron.Energy *= 0.9f;

			if (Electron.Energy < EnergyThreshold)
			{
				FinalPositions.Add(Electron.Position);
				Electrons.RemoveAt(Index);
			}

			continue;
		}
	}

	for (const FScatteredElectron& Electron : Electrons)
	{
		DrawDebugLine(GetWorld(), Electron.Position, Electron.Position + Electron.Direction * StepDistance, FColor::Cyan, false, -1.0f, 0, 1.0f);
	}
}
