// Copyright Epic Games, Inc.

#include "QuantumVisualizerComponent.h"

#include "Components/MeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UQuantumVisualizerComponent::UQuantumVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumVisualizerComponent::InitializeQubits(int32 NumQubits)
{
	// Destroy existing visuals
	for (TWeakObjectPtr<AActor>& ActorPtr : QubitActors)
	{
		if (ActorPtr.IsValid())
		{
			ActorPtr->Destroy();
		}
	}
	QubitActors.Reset();

	if (!GetWorld() || NumQubits <= 0)
	{
		return;
	}

	for (int32 i = 0; i < NumQubits; ++i)
	{
		const FVector Location(i * QubitSpacing, 0.f, 0.f);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* Qubit = GetWorld()->SpawnActor<AActor>(QubitActorClass ? *QubitActorClass : AActor::StaticClass(), Location, FRotator::ZeroRotator, Params);
		if (Qubit)
		{
#if WITH_EDITOR
			Qubit->SetActorLabel(FString::Printf(TEXT("Qubit_%d"), i));
#endif
			QubitActors.Add(Qubit);
		}
	}
}

void UQuantumVisualizerComponent::VisualizePulse(double Amplitude, double Frequency, double Duration)
{
	UE_LOG(LogTemp, Log, TEXT("Visualizing Pulse: Amplitude=%f, Frequency=%f, Duration=%f"), Amplitude, Frequency, Duration);
	// Placeholder: hook into a particle or mesh effect if desired.
}

void UQuantumVisualizerComponent::VisualizeGate(const FQuantumGate& Gate)
{
	if (!GetWorld() || QubitActors.Num() == 0)
	{
		return;
	}

	FVector Position = FVector::ZeroVector;
	int32 ValidCount = 0;

	// Use PrimaryIndex and SecondaryIndex where applicable
	TArray<int32> GateQubits;
	if (Gate.PrimaryIndex >= 0)
	{
		GateQubits.Add(Gate.PrimaryIndex);
	}
	if (Gate.SecondaryIndex >= 0)
	{
		GateQubits.Add(Gate.SecondaryIndex);
	}

	for (int32 QubitIdx : GateQubits)
	{
		if (QubitActors.IsValidIndex(QubitIdx) && QubitActors[QubitIdx].IsValid())
		{
			Position += QubitActors[QubitIdx]->GetActorLocation();
			++ValidCount;
		}
	}

	if (ValidCount == 0)
	{
		return;
	}

	Position /= static_cast<float>(ValidCount);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* GateActor = GetWorld()->SpawnActor<AActor>(GateActorClass ? *GateActorClass : AActor::StaticClass(), Position, FRotator::ZeroRotator, Params);
	if (GateActor)
	{
#if WITH_EDITOR
		GateActor->SetActorLabel(TEXT("Gate"));
#endif
	}
}

void UQuantumVisualizerComponent::ShowNoiseEffect(int32 QubitIndex)
{
	if (!QubitActors.IsValidIndex(QubitIndex) || !QubitActors[QubitIndex].IsValid())
	{
		return;
	}

	AActor* Qubit = QubitActors[QubitIndex].Get();
	if (UPrimitiveComponent* Prim = Qubit->FindComponentByClass<UPrimitiveComponent>())
	{
		if (UMeshComponent* Mesh = Cast<UMeshComponent>(Prim))
		{
			Mesh->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), FVector(1.f, 0.f, 0.f));
		}
	}

	// Fallback: draw a debug sphere to indicate noise
	DrawDebugSphere(GetWorld(), Qubit->GetActorLocation(), 25.f, 12, FColor::Red, false, 1.5f);
	UE_LOG(LogTemp, Log, TEXT("Noise applied to Qubit %d"), QubitIndex);
}
