#include "LeylineEnergyVisualizer2Actor.h"
#include "Engine/Engine.h"
#include "Engine/CollisionProfile.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

ALeylineEnergyVisualizer2Actor::ALeylineEnergyVisualizer2Actor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ParticleInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ParticleInstances"));
	ParticleInstances->SetupAttachment(RootComponent);
	ParticleInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ALeylineEnergyVisualizer2Actor::BeginPlay()
{
	Super::BeginPlay();

	if (!ParticleMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("ParticleMesh not configured; visualization skipped."));
		return;
	}

	ParticleInstances->SetStaticMesh(ParticleMesh);
	if (ParticleMaterial)
	{
		ParticleInstances->SetMaterial(0, ParticleMaterial);
	}

	GenerateEnergyFlowValues();

	if (EnergyFlowValues.Num() == 0)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ALeylineEnergyVisualizer2Actor::SpawnNextParticle, TimeBetweenSpawns, true);
}

void ALeylineEnergyVisualizer2Actor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void ALeylineEnergyVisualizer2Actor::GenerateEnergyFlowValues()
{
	EnergyFlowValues.Reset();
	if (Steps <= 0)
	{
		return;
	}

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float T = FMath::Lerp(StartTime, EndTime, static_cast<float>(Index) / FMath::Max(1, Steps - 1));
		const double Value = ComputeEnergyFlow(T, T + 0.1, IntegrationParameter);
		EnergyFlowValues.Add(Value);
	}
	CurrentIndex = 0;
}

void ALeylineEnergyVisualizer2Actor::SpawnNextParticle()
{
	if (CurrentIndex >= EnergyFlowValues.Num())
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	const float X = CurrentIndex;
	const float Y = static_cast<float>(EnergyFlowValues[CurrentIndex]);
	const FVector Location = GetActorLocation() + FVector(X * 100.0f, Y * 100.0f, 0.0f);
	const FTransform Transform(FRotator::ZeroRotator, Location, FVector(0.5f));
	ParticleInstances->AddInstance(Transform);

	++CurrentIndex;
}

double ALeylineEnergyVisualizer2Actor::ComputeEnergyFlow(double Start, double End, int32 Parameter) const
{
	return (End - Start) * Parameter;
}
