#include "NanoassemblyControllerComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

UNanoassemblyControllerComponent::UNanoassemblyControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNanoassemblyControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	ParticlePositions.SetNumZeroed(SimulatedParticleCount);
	ParticleVelocities.SetNumZeroed(SimulatedParticleCount);

	for (int32 Index = 0; Index < SimulatedParticleCount; ++Index)
	{
		ParticlePositions[Index] = GetOwner()->GetActorLocation() + FMath::VRand() * 100.0f;
	}

	SpatialPartition.Initialize(CellSize);
}

void UNanoassemblyControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TargetPositions.Num() == 0 || DeltaTime <= 0.0f)
	{
		return;
	}

	SpatialPartition.Clear();

	for (int32 Index = 0; Index < ParticlePositions.Num(); ++Index)
	{
		SpatialPartition.AddParticle(ParticlePositions[Index], Index);
	}

	const int32 Steps = FMath::Min(ParticlePositions.Num(), TargetPositions.Num());

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		FVector& ParticlePos = ParticlePositions[Index];
		FVector& Velocity = ParticleVelocities[Index];
		const FVector Target = TargetPositions[Index];

		const FVector Direction = (Target - ParticlePos).GetSafeNormal();
		const FVector Force = Direction * AssemblyForce * DeltaTime;

		Velocity += Force;
		ParticlePos += Velocity * DeltaTime;

		if (FVector::Dist(ParticlePos, Target) < 10.0f)
		{
			ParticlePos = Target;
			Velocity = FVector::ZeroVector;
		}
	}

	if (GetWorld())
	{
		for (const FVector& Target : TargetPositions)
		{
			DrawDebugSphere(GetWorld(), Target, 5.0f, 8, FColor::Green, false, -1.0f);
		}
		if (SimulatedParticleCount > 0)
		{
			for (int32 Index = 0; Index < ParticlePositions.Num(); ++Index)
			{
				DrawDebugPoint(GetWorld(), ParticlePositions[Index], 5.0f, FColor::Cyan, false, -1.0f);
			}
		}
	}
}

void UNanoassemblyControllerComponent::AddTargetPosition(const FVector& Position)
{
	TargetPositions.Add(Position);
}

void UNanoassemblyControllerComponent::RemoveTargetPosition(const FVector& Position)
{
	TargetPositions.RemoveSingle(Position);
}

void UNanoassemblyControllerComponent::ClearTargetPositions()
{
	TargetPositions.Empty();
}
