#include "MemoryEncodingActor.h"
#include "Math/UnrealMathUtility.h"

AMemoryEncodingActor::AMemoryEncodingActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMemoryEncodingActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeMemoryStrength();
}

void AMemoryEncodingActor::ComputeMemoryStrength() const
{
	const float Strength = InitialStrength * FMath::Exp(-ForgettingRate * TimeSeconds);
	UE_LOG(LogTemp, Log, TEXT("Memory Strength at t=%.2f: %.4f"), TimeSeconds, Strength);
}
