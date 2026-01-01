// Copyright Epic Games, Inc.

#include "EntanglementManagerComponent.h"

FVector2D UEntanglementManagerComponent::Entangle(const FVector2D& SuperposedState, float Flux)
{
	UE_LOG(LogTemp, Log, TEXT("Entangling state (%f, %f) with flux %f"), SuperposedState.X, SuperposedState.Y, Flux);

	const float NormalizedFlux = FMath::Clamp(Flux / 10.0f, 0.0f, 1.0f);
	UE_LOG(LogTemp, Log, TEXT("Normalized Flux: %f"), NormalizedFlux);

	const FVector2D EntangledStateA = SuperposedState * NormalizedFlux;
	const FVector2D EntangledStateB = FVector2D(-SuperposedState.X, -SuperposedState.Y) * (1.0f - NormalizedFlux);

	const FVector2D FinalEntangledState = FMath::Lerp(EntangledStateA, EntangledStateB, NormalizedFlux);

	UE_LOG(LogTemp, Log, TEXT("Entangled State A: (%f, %f)"), EntangledStateA.X, EntangledStateA.Y);
	UE_LOG(LogTemp, Log, TEXT("Entangled State B: (%f, %f)"), EntangledStateB.X, EntangledStateB.Y);
	UE_LOG(LogTemp, Log, TEXT("Final Entangled State: (%f, %f)"), FinalEntangledState.X, FinalEntangledState.Y);

	return FinalEntangledState;
}
