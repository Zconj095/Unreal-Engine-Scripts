// Copyright Epic Games, Inc.

#include "BlochSphereVisualizerComponent.h"

#include "Components/MeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"

UBlochSphereVisualizerComponent::UBlochSphereVisualizerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBlochSphereVisualizerComponent::VisualizeStatePositions(const TArray<FVector>& QubitStates)
{
    if (QubitStates.Num() != NumQubits)
    {
        UE_LOG(LogTemp, Error, TEXT("Mismatch between number of qubits (%d) and provided states (%d)."), NumQubits, QubitStates.Num());
        return;
    }

    for (int32 Index = 0; Index < NumQubits; ++Index)
    {
        const FVector Position = QubitStates[Index];
        const float T = FMath::Clamp((Position.Z + 1.0f) * 0.5f, 0.0f, 1.0f); // map -1..1 to 0..1
        const FLinearColor Color = FLinearColor::LerpUsingHSV(FLinearColor::Blue, FLinearColor::Red, T);
        SpawnSphereAt(Position, Color, Index);
        UE_LOG(LogTemp, Log, TEXT("Qubit %d visualized at position: %s"), Index, *Position.ToString());
    }
}

void UBlochSphereVisualizerComponent::VisualizeStateVector(const FBlochQuantumState& State)
{
    const int32 Count = State.StateVector.Num();
    if (Count == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("VisualizeStateVector called with empty state vector."));
        return;
    }

    for (int32 Index = 0; Index < Count; ++Index)
    {
        const FVector Position = FVector(Index * VectorSpacing, 0.f, 0.f);
        const FBlochQuantumComplex& Amp = State.StateVector[Index];
        const FLinearColor Color(Amp.Real, 0.0f, Amp.Imag, 1.0f);
        SpawnSphereAt(Position, Color, Index);
    }
}

void UBlochSphereVisualizerComponent::SpawnSphereAt(const FVector& Location, const FLinearColor& Color, int32 Index)
{
    UWorld* World = GetWorld();
    if (!World || !SphereActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot spawn sphere: missing world or SphereActorClass."));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor* SphereActor = World->SpawnActor<AActor>(SphereActorClass, Location, FRotator::ZeroRotator, SpawnParams);
    if (!SphereActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn sphere actor for index %d."), Index);
        return;
    }

    SphereActor->SetActorScale3D(SphereScale);
#if WITH_EDITOR
    SphereActor->SetActorLabel(FString::Printf(TEXT("Qubit_%d_Sphere"), Index));
#endif

    TArray<UMeshComponent*> MeshComponents;
    SphereActor->GetComponents<UMeshComponent>(MeshComponents);
    for (UMeshComponent* Mesh : MeshComponents)
    {
        if (!Mesh)
        {
            continue;
        }

        const int32 MatCount = Mesh->GetNumMaterials();
        for (int32 MatIdx = 0; MatIdx < MatCount; ++MatIdx)
        {
            if (UMaterialInstanceDynamic* MID = Mesh->CreateAndSetMaterialInstanceDynamic(MatIdx))
            {
                MID->SetVectorParameterValue(ColorParameterName, Color);
            }
        }
    }
}
