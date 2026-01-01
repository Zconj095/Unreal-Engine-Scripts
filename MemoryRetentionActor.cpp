#include "MemoryRetentionActor.h"
#include "Math/UnrealMathUtility.h"

AMemoryRetentionActor::AMemoryRetentionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMemoryRetentionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	const float Retention = CalculateRetention(ElapsedTime);
	UE_LOG(LogTemp, Log, TEXT("Memory Retention at time %.2f: %.4f"), ElapsedTime, Retention);
}

float AMemoryRetentionActor::CalculateRetention(float TimeSeconds) const
{
	const float FirstTerm = InitialCapacity * FMath::Exp(-DecayRate * TimeSeconds);
	const float SecondTerm = EventBoost * FMath::Exp(-BoostSharpness * FMath::Pow(TimeSeconds - PeakTime, 2));
	return FirstTerm + SecondTerm;
}
