#include "ReactionRateVisualizerComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

UReactionRateVisualizerComponent::UReactionRateVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Temperatures = {300.0f, 310.0f, 320.0f, 330.0f, 340.0f};
	MagicalFields = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f};
}

void UReactionRateVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Temperatures.Num() != MagicalFields.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Temperatures and MagicalFields must be the same length."));
		return;
	}

	const FVector Origin = GetOwner()->GetActorLocation();
	for (int32 Index = 0; Index < Temperatures.Num(); ++Index)
	{
		const float Temperature = Temperatures[Index];
		const float MagicalField = MagicalFields[Index];
		const float Rate = FrequencyFactor * FMath::Exp(-ActivationEnergy / (GasConstant * Temperature)) + Gamma * MagicalField;
		const FVector Position = Origin + FVector(Index * 100.0f, 0.0f, Rate * HeightScale);

		UE_LOG(LogTemp, Log, TEXT("Reaction Rate [%d]: %0.4f"), Index + 1, Rate);
		DrawDebugSphere(GetWorld(), Position, 25.0f, 12, FColor::Emerald, true, 0.0f, 0, 2.0f);
	}
}
