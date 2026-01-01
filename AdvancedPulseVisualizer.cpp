// Copyright Epic Games, Inc.

#include "AdvancedPulseVisualizer.h"

#include "Components/MeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"

AAdvancedPulseVisualizer::AAdvancedPulseVisualizer()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AAdvancedPulseVisualizer::BeginPlay()
{
    Super::BeginPlay();
}

void AAdvancedPulseVisualizer::VisualizePulse(float Amplitude, float Frequency, float Duration)
{
    UWorld* World = GetWorld();
    if (!World || !PulseActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot visualize pulse: missing world or PulseActorClass."));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor* PulseActor = World->SpawnActor<AActor>(PulseActorClass, SpawnLocation, SpawnRotation, SpawnParams);
    if (!PulseActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn pulse actor."));
        return;
    }

    PulseActor->SetActorScale3D(FVector(Amplitude, Frequency, Duration));

#if WITH_EDITOR
    PulseActor->SetActorLabel(FString::Printf(TEXT("Pulse_A%.2f_F%.2f_D%.2f"), Amplitude, Frequency, Duration));
#endif

    const FLinearColor Color(
        Amplitude / 10.0f,
        Frequency / 100.0f,
        Duration / 5.0f);

    TArray<UMeshComponent*> MeshComponents;
    PulseActor->GetComponents<UMeshComponent>(MeshComponents);

    for (UMeshComponent* Mesh : MeshComponents)
    {
        if (!Mesh)
        {
            continue;
        }

        const int32 MaterialCount = Mesh->GetNumMaterials();
        for (int32 Index = 0; Index < MaterialCount; ++Index)
        {
            if (UMaterialInstanceDynamic* MID = Mesh->CreateAndSetMaterialInstanceDynamic(Index))
            {
                MID->SetVectorParameterValue(ColorParameterName, Color);
            }
        }
    }
}
