#include "ParticleSimulationGridComponent.h"
#include "DrawDebugHelpers.h"

UParticleSimulationGridComponent::UParticleSimulationGridComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UParticleSimulationGridComponent::BeginPlay()
{
    Super::BeginPlay();
    SpatialGrid.Initialize(CellSize);
    InitializeParticles();
}

void UParticleSimulationGridComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Particles.Num() == 0 || DeltaTime <= 0.0f)
    {
        return;
    }

    SpatialGrid.Clear();
    UpdateParticles(DeltaTime);

    if (bDrawDebug)
    {
        DrawDebugSpheres();
    }
}

void UParticleSimulationGridComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    Particles.Empty();
    Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UParticleSimulationGridComponent::InitializeParticles()
{
    Particles.SetNumZeroed(ParticleCount);
    for (int32 Index = 0; Index < ParticleCount; ++Index)
    {
        Particles[Index].Position = GetOwner()->GetActorLocation() + FMath::VRand() * 200.0f;
        Particles[Index].Velocity = FVector::ZeroVector;
        SpatialGrid.AddParticle(Particles[Index].Position, Index);
    }
}

void UParticleSimulationGridComponent::UpdateParticles(float DeltaTime)
{
    for (int32 Index = 0; Index < Particles.Num(); ++Index)
    {
        SpatialGrid.AddParticle(Particles[Index].Position, Index);
    }

    for (int32 Index = 0; Index < Particles.Num(); ++Index)
    {
        FSimulatedParticle& Particle = Particles[Index];
        const TArray<int32> Neighbors = SpatialGrid.QueryNeighbors(Particle.Position);

        for (int32 NeighborIndex : Neighbors)
        {
            if (NeighborIndex == Index)
            {
                continue;
            }

            const FVector Delta = Particles[NeighborIndex].Position - Particle.Position;
            const float Distance = Delta.Size();
            if (Distance <= KINDA_SMALL_NUMBER)
            {
                continue;
            }

            if (Distance < InteractionRadius)
            {
                const FVector Direction = Delta / Distance;
                const float Strength = (InteractionRadius - Distance) * DeltaTime * 0.5f;
                Particle.Velocity -= Direction * Strength;
            }
        }

        Particle.Position += Particle.Velocity * DeltaTime;
    }
}

void UParticleSimulationGridComponent::DrawDebugSpheres() const
{
    if (!GetWorld())
    {
        return;
    }

    for (const FSimulatedParticle& Particle : Particles)
    {
        DrawDebugSphere(GetWorld(), Particle.Position, InteractionRadius, 8, FColor::Yellow, false, -1.0f, 0, 0.5f);
    }
}
