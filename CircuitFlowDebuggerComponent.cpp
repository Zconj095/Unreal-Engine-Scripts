// Copyright Epic Games, Inc.

#include "CircuitFlowDebuggerComponent.h"

#include "Components/MeshComponent.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

UCircuitFlowDebuggerComponent::UCircuitFlowDebuggerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCircuitFlowDebuggerComponent::HighlightGateByName(const FString& GateName, FLinearColor HighlightColor, float DurationSeconds)
{
    UE_LOG(LogTemp, Log, TEXT("Applying gate: %s"), *GateName);

    AActor* GateActor = FindGateActor(GateName);
    if (!GateActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Gate actor '%s' not found."), *GateName);
        return;
    }

    SetActorColor(GateActor, HighlightColor);

    if (DurationSeconds > 0.0f)
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateUObject(this, &UCircuitFlowDebuggerComponent::ResetColor, GateActor),
            DurationSeconds,
            false);
    }
}

AActor* UCircuitFlowDebuggerComponent::FindGateActor(const FString& GateName) const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor)
        {
            continue;
        }

        const FString ActorName = Actor->GetName();
        if (ActorName.Equals(GateName, ESearchCase::IgnoreCase))
        {
            return Actor;
        }

#if WITH_EDITOR
        const FString ActorLabel = Actor->GetActorLabel(false);
        if (ActorLabel.Equals(GateName, ESearchCase::IgnoreCase))
        {
            return Actor;
        }
#endif
    }

    return nullptr;
}

void UCircuitFlowDebuggerComponent::SetActorColor(AActor* Actor, const FLinearColor& Color)
{
    if (!Actor)
    {
        return;
    }

    TArray<UMeshComponent*> MeshComponents;
    Actor->GetComponents<UMeshComponent>(MeshComponents);

    for (UMeshComponent* Mesh : MeshComponents)
    {
        if (!Mesh)
        {
            continue;
        }

        const int32 MatCount = Mesh->GetNumMaterials();
        for (int32 Index = 0; Index < MatCount; ++Index)
        {
            if (UMaterialInstanceDynamic* MID = Mesh->CreateAndSetMaterialInstanceDynamic(Index))
            {
                MID->SetVectorParameterValue(ColorParameterName, Color);
            }
        }
    }
}

void UCircuitFlowDebuggerComponent::ResetColor(AActor* Actor)
{
    // Default reset to white, similar to Unity implementation.
    SetActorColor(Actor, FLinearColor::White);
}
