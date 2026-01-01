#include "ReactionVelocityVisualizerComponent.h"
#include "DrawDebugHelpers.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UReactionVelocityVisualizerComponent::UReactionVelocityVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SubstrateConcentrations = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
	MagicalEnergies = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f};
}

void UReactionVelocityVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SubstrateConcentrations.Num() != MagicalEnergies.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Substrate and magical arrays must have equal length."));
		return;
	}

	ReactionVelocities.Empty();
	const FVector Origin = GetOwner()->GetActorLocation() + FVector::UpVector * HeightScale;

	for (int32 Index = 0; Index < SubstrateConcentrations.Num(); ++Index)
	{
		const float Velocity = (Vmax * SubstrateConcentrations[Index]) / (Km + SubstrateConcentrations[Index]) + Lambda * MagicalEnergies[Index];
		ReactionVelocities.Add(Velocity);

		const FVector Position = Origin + FVector(Index * Spacing, 0.0f, Velocity * HeightScale);
		DrawDebugSphere(GetWorld(), Position, 25.0f, 16, SphereColor, true, 0.0f, 0, 2.0f);
		UE_LOG(LogTemp, Log, TEXT("Reaction Velocity [%d]: %0.4f"), Index + 1, Velocity);
	}
}
