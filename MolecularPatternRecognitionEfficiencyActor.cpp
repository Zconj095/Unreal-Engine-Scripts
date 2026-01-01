#include "MolecularPatternRecognitionEfficiencyActor.h"
#include "Math/UnrealMathUtility.h"

AMolecularPatternRecognitionEfficiencyActor::AMolecularPatternRecognitionEfficiencyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMolecularPatternRecognitionEfficiencyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	if (TimeMs == 0.f || Sigma == 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("T and σ must not be zero to calculate MPRE."));
		return;
	}

	const float Mpre = CalculateMPRE();
	UE_LOG(LogTemp, Log, TEXT("[%.2f] MPRE Result: %.4f"), ElapsedTime, Mpre);
}

float AMolecularPatternRecognitionEfficiencyActor::CalculateMPRE() const
{
	return Alpha * (Phi / (TimeMs * Sigma));
}
