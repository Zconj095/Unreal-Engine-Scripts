// Copyright Epic Games, Inc.

#include "GateDragHandlerComponent.h"

#include "DynamicCircuitEditorComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UGateDragHandlerComponent::UGateDragHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGateDragHandlerComponent::OnBeginDrag()
{
	UE_LOG(LogTemp, Log, TEXT("Started dragging gate..."));
}

void UGateDragHandlerComponent::OnDrag(const FVector2D& ScreenDelta)
{
	// Optional: update UI drag visuals externally; here we only log.
	UE_LOG(LogTemp, Verbose, TEXT("Dragging gate, delta: (%f, %f)"), ScreenDelta.X, ScreenDelta.Y);
}

void UGateDragHandlerComponent::OnEndDrag(const FString& GateName, const FVector2D& ScreenPosition)
{
	EnsureCircuitEditor();

	AActor* QubitActor = DetectQubit(ScreenPosition);
	if (!QubitActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No qubit detected under drop location."));
		return;
	}

	const int32 QubitIndex = GetQubitIndex(QubitActor);
	if (QubitIndex < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid qubit index detected."));
		return;
	}

	AttachGateToCircuit(GateName, QubitIndex);
}

AActor* UGateDragHandlerComponent::DetectQubit(const FVector2D& ScreenPosition) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		return nullptr;
	}

	FVector WorldOrigin;
	FVector WorldDirection;
	if (!PC->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldOrigin, WorldDirection))
	{
		return nullptr;
	}

	const FVector TraceStart = WorldOrigin;
	const FVector TraceEnd = TraceStart + WorldDirection * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(GateDragTrace), true);
	if (World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		if (Hit.GetActor() && Hit.GetActor()->ActorHasTag(QubitTag))
		{
			return Hit.GetActor();
		}
	}

	return nullptr;
}

int32 UGateDragHandlerComponent::GetQubitIndex(AActor* QubitActor) const
{
	if (!QubitActor)
	{
		return -1;
	}

	const FString Name = QubitActor->GetName();
	TArray<FString> Parts;
	Name.ParseIntoArrayWS(Parts);
	if (Parts.Num() == 2)
	{
		int32 Index = -1;
		if (Parts[1].IsNumeric())
		{
			Index = FCString::Atoi(*Parts[1]);
			return Index;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Could not parse qubit index from object name '%s'."), *Name);
	return -1;
}

void UGateDragHandlerComponent::AttachGateToCircuit(const FString& GateName, int32 QubitIndex)
{
	if (!CircuitEditor)
	{
		UE_LOG(LogTemp, Warning, TEXT("CircuitEditor not set; cannot attach gate."));
		return;
	}

	// Only Hadamard and CNOT are currently supported by the shared FQuantumGate struct.
	if (GateName.Equals(TEXT("Hadamard"), ESearchCase::IgnoreCase))
	{
		CircuitEditor->AddGate(TEXT("Hadamard"), QubitIndex, -1);
		UE_LOG(LogTemp, Log, TEXT("%s gate added to qubit %d"), *GateName, QubitIndex);
	}
	else if (GateName.Equals(TEXT("CNOT"), ESearchCase::IgnoreCase))
	{
		if (CircuitEditor->NumQubits > 1)
		{
			CircuitEditor->AddGate(TEXT("CNOT"), QubitIndex, DefaultControlQubit);
			UE_LOG(LogTemp, Log, TEXT("%s gate added: control %d, target %d"), *GateName, DefaultControlQubit, QubitIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CNOT requires at least 2 qubits."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown or unsupported gate name: %s"), *GateName);
	}
}

void UGateDragHandlerComponent::EnsureCircuitEditor()
{
	if (CircuitEditor)
	{
		return;
	}

	if (AActor* Owner = GetOwner())
	{
		CircuitEditor = Owner->FindComponentByClass<UDynamicCircuitEditorComponent>();
	}
}
