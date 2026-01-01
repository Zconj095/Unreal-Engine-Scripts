#include "MemoryRetention2Actor.h"
#include "Math/UnrealMathUtility.h"

AMemoryRetention2Actor::AMemoryRetention2Actor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMemoryRetention2Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;
	const float Retention = CalculateRetention(TimeElapsed);
	UE_LOG(LogTemp, Log, TEXT("Memory Retention at time %.2f: %.4f"), TimeElapsed, Retention);
}

float AMemoryRetention2Actor::CalculateRetention(float Seconds) const
{
	const float DecayTerm = InitialCapacity * FMath::Exp(-DecayRate * Seconds);
	const float PeakTerm = EventBoost * FMath::Exp(-Sensitivity * FMath::Pow(Seconds - PeakTime, 2));
	return DecayTerm + PeakTerm;
}
