// Copyright Epic Games, Inc.

#include "DynamicBlochSphereComponent.h"

#include "Components/MeshComponent.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"

UDynamicBlochSphereComponent::UDynamicBlochSphereComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDynamicBlochSphereComponent::UpdateBlochSphere(const TArray<FComplexNumber>& StateVector)
{
    UWorld* World = GetWorld();
    if (!World || !SphereActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot update Bloch spheres: missing world or SphereActorClass."));
        return;
    }

    for (int32 Index = 0; Index < StateVector.Num(); ++Index)
    {
        const FVector Location(Index * Spacing, 0.f, 0.f);
        AActor* SphereActor = FindOrSpawnSphere(Index, Location);
        if (!SphereActor)
        {
            continue;
        }

        const FComplexNumber& Amp = StateVector[Index];
        const FLinearColor Color(
            FMath::Abs(static_cast<float>(Amp.Real)),
            FMath::Abs(static_cast<float>(Amp.Imaginary)),
            1.0f - FMath::Abs(static_cast<float>(Amp.Magnitude())));

        SetSphereColor(SphereActor, Color);
    }
}

AActor* UDynamicBlochSphereComponent::FindOrSpawnSphere(int32 Index, const FVector& Location)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    AActor* SphereActor = nullptr;

    // Try to find an existing actor with the expected name.
    const FString TargetName = FString::Printf(TEXT("Qubit %d Bloch Sphere"), Index);
    for (TActorIterator<AActor> It(World); It; ++It)
    {
        if (It->GetName().Equals(TargetName, ESearchCase::IgnoreCase))
        {
            SphereActor = *It;
            break;
        }
#if WITH_EDITOR
        if (It->GetActorLabel(false).Equals(TargetName, ESearchCase::IgnoreCase))
        {
            SphereActor = *It;
            break;
        }
#endif
    }

    if (!SphereActor)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        SphereActor = World->SpawnActor<AActor>(SphereActorClass, Location, FRotator::ZeroRotator, SpawnParams);
        if (SphereActor)
        {
            SphereActor->SetActorScale3D(SphereScale);
#if WITH_EDITOR
            SphereActor->SetActorLabel(TargetName);
#endif
            SphereActor->Rename(*TargetName);
        }
    }
    else
    {
        SphereActor->SetActorLocation(Location);
    }

    return SphereActor;
}

void UDynamicBlochSphereComponent::SetSphereColor(AActor* SphereActor, const FLinearColor& Color)
{
    if (!SphereActor)
    {
        return;
    }

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
