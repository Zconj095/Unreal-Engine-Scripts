#include "MoonlightMemoryEncodingActor.h"
#include "Math/UnrealMathUtility.h"

AMoonlightMemoryEncodingActor::AMoonlightMemoryEncodingActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMoonlightMemoryEncodingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	const float Capacity = ComputeMME();
	UE_LOG(LogTemp, Log, TEXT("[%.2f] Moonlight Memory Capacity (Ml): %.4f"), ElapsedTime, Capacity);
}

float AMoonlightMemoryEncodingActor::ComputeMME() const
{
	const float PhaseFactor = FMath::Sin(PI * LunarPhase / PhasePeriod);
	return EncodingEfficiency * PhaseFactor * PhaseFactor * EncodedPatterns;
}
