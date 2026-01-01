#include "LeyFieldResonanceActor.h"
#include "LeyFieldResonanceUtility.h"
#include "Engine/Engine.h"

ALeyFieldResonanceActor::ALeyFieldResonanceActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeyFieldResonanceActor::BeginPlay()
{
	Super::BeginPlay();

	auto PhiFunction = [](const FVector& Position) -> double
	{
		return FMath::Exp(-Position.Size());
	};

	auto LeyFieldFunction = [](const FVector& Position) -> FVector
	{
		return FVector(FMath::Sin(Position.X), FMath::Cos(Position.Y), FMath::Sin(Position.Z));
	};

	double Energy = ULeyFieldResonanceUtility::ComputeLeyFieldEnergy(PhiFunction, LeyFieldFunction, MinBounds, MaxBounds, Resolution);
	UE_LOG(LogTemp, Log, TEXT("Ley Field Resonance Energy: %.6f"), Energy);
}
