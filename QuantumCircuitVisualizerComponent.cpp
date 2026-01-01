// Copyright Epic Games, Inc.

#include "QuantumCircuitVisualizerComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/LineBatchComponent.h"
#include "DrawDebugHelpers.h"

UQuantumCircuitVisualizerComponent::UQuantumCircuitVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumCircuitVisualizerComponent::InitializeCircuitVisualization()
{
	if (AActor* Owner = GetOwner())
	{
		for (TWeakObjectPtr<AActor>& SpherePtr : QubitSpheres)
		{
			if (SpherePtr.IsValid())
			{
				SpherePtr->Destroy();
			}
		}
		QubitSpheres.Reset();

		for (int32 i = 0; i < NumQubits; ++i)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AActor* Sphere = Owner->GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
			if (Sphere)
			{
				Sphere->SetActorLocation(FVector(0.f, i * GateSpacing, 0.f));
				Sphere->SetActorScale3D(FVector(50.f));
#if WITH_EDITOR
				Sphere->SetActorLabel(FString::Printf(TEXT("Qubit_%d"), i));
#endif
				QubitSpheres.Add(Sphere);
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Initialized quantum circuit visualization."));
	}
}

void UQuantumCircuitVisualizerComponent::AddGate(const FString& GateType, int32 TargetQubit, int32 ControlQubit)
{
	if (!GetWorld())
	{
		return;
	}

	const FVector GatePosition(GateSpacing, TargetQubit * GateSpacing, 0.f);

	if (ControlQubit >= 0)
	{
		const FVector ControlPosition(GateSpacing, ControlQubit * GateSpacing, 0.f);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* ControlSphere = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), ControlPosition, FRotator::ZeroRotator, Params);
		if (ControlSphere)
		{
			ControlSphere->SetActorScale3D(FVector(20.f));
		}

		DrawLine(ControlPosition, GatePosition, FLinearColor::Yellow);
	}

	// Gate cube
	FActorSpawnParameters GateParams;
	GateParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* GateActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), GatePosition, FRotator::ZeroRotator, GateParams);
	if (GateActor)
	{
		GateActor->SetActorScale3D(FVector(50.f));
#if WITH_EDITOR
		GateActor->SetActorLabel(FString::Printf(TEXT("%s_Gate"), *GateType));
#endif
	}

	UE_LOG(LogTemp, Log, TEXT("Added %s gate to Qubit %d."), *GateType, TargetQubit);
}

void UQuantumCircuitVisualizerComponent::DrawLine(const FVector& Start, const FVector& End, const FLinearColor& Color)
{
	DrawDebugLine(GetWorld(), Start, End, Color.ToFColor(true), false, 5.0f, 0, 5.0f);
}
