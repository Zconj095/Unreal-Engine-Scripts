#include "GeochemicalPatternRecognitionEfficiencyActor.h"

AGeochemicalPatternRecognitionEfficiencyActor::AGeochemicalPatternRecognitionEfficiencyActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeochemicalPatternRecognitionEfficiencyActor::BeginPlay()
{
	Super::BeginPlay();

	GPREValue = CalculateGPRE(Alpha, Phi, RecognitionTime, SynapticDensity);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Geochemical Pattern Recognition Efficiency (GPRE): %.4f"), GPREValue);
	}
}

float AGeochemicalPatternRecognitionEfficiencyActor::CalculateGPRE(float InAlpha, float InPhi, float InRecognitionTime, float InSynapticDensity) const
{
	if (FMath::IsNearlyZero(InRecognitionTime) || FMath::IsNearlyZero(InSynapticDensity))
	{
		UE_LOG(LogTemp, Error, TEXT("Recognition time and synaptic density must be non-zero."));
		return 0.0f;
	}

	return InAlpha * (InPhi / (InRecognitionTime * InSynapticDensity));
}

float AGeochemicalPatternRecognitionEfficiencyActor::GetGPRE() const
{
	return GPREValue;
}
