#include "NanoFieldSimulatorComponent.h"
#include "DrawDebugHelpers.h"

UNanoFieldSimulatorComponent::UNanoFieldSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNanoFieldSimulatorComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeParticles();
}

void UNanoFieldSimulatorComponent::InitializeParticles()
{
	ParticlePositions.SetNumZeroed(ParticleCount);
	ParticleVelocities.SetNumZeroed(ParticleCount);
	for (int32 Index = 0; Index < ParticleCount; ++Index)
	{
		ParticlePositions[Index] = GetOwner()->GetActorLocation() + FMath::VRand() * 200.0f;
		ParticleVelocities[Index] = FVector::ZeroVector;
	}
}

void UNanoFieldSimulatorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FieldSources.Num() == 0 || ParticleCount == 0 || DeltaTime <= 0.0f)
	{
		return;
	}

	ApplyFieldToParticles(DeltaTime);
	DrawFieldGizmos();
}

void UNanoFieldSimulatorComponent::ApplyFieldToParticles(float DeltaTime)
{
	for (int32 Index = 0; Index < ParticlePositions.Num(); ++Index)
	{
		FVector& Position = ParticlePositions[Index];
		FVector& Velocity = ParticleVelocities[Index];

		FVector TotalField = FVector::ZeroVector;
		for (const FFieldSource& Source : FieldSources)
		{
			TotalField += Source.CalculateField(Position);
		}

		Velocity += TotalField * InfluenceMultiplier * DeltaTime;
		Position += Velocity * DeltaTime;
	}
}

void UNanoFieldSimulatorComponent::DrawFieldGizmos() const
{
	if (!GetWorld())
	{
		return;
	}

	for (const FFieldSource& Source : FieldSources)
	{
		DrawDebugSphere(GetWorld(), Source.Position, Source.InteractionRadius, 16, FColor::Orange, false, -1.0f, 0, 2.0f);
	}

	for (const FVector& Particle : ParticlePositions)
	{
		DrawDebugPoint(GetWorld(), Particle, 5.0f, FColor::Cyan, false, -1.0f);
	}
}
