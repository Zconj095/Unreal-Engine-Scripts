#include "MemoryReconstructionActor.h"
#include "Math/UnrealMathUtility.h"

AMemoryReconstructionActor::AMemoryReconstructionActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMemoryReconstructionActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeReconstruction();
}

float AMemoryReconstructionActor::Phi(float S) const
{
	return FMath::Sin(S) + 2.f;
}

void AMemoryReconstructionActor::ComputeReconstruction() const
{
	double Memory = 0.0;
	for (float S = StartTimeSeconds; S <= EndTimeSeconds; S += TimeStep)
	{
		const double Intensity = Phi(S);
		const double Decay = FMath::Exp(-ForgettingRate * (EndTimeSeconds - S));
		Memory += Intensity * Decay * TimeStep;
	}

	UE_LOG(LogTemp, Log, TEXT("Memory at time t (M_t): %.4f"), Memory);
}
