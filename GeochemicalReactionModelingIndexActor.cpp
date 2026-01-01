#include "GeochemicalReactionModelingIndexActor.h"

#include "Math/UnrealMathUtility.h"

AGeochemicalReactionModelingIndexActor::AGeochemicalReactionModelingIndexActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeochemicalReactionModelingIndexActor::BeginPlay()
{
	Super::BeginPlay();

	GRMIValue = CalculateGRMI(Kappa, DeltaR, ModelingTime, CognitiveEnergy);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Geochemical Reaction Modeling Index (GRMI): %.4f"), GRMIValue);
	}
}

float AGeochemicalReactionModelingIndexActor::CalculateGRMI(float InKappa, float InDeltaR, float InModelingTime, float InCognitiveEnergy) const
{
	if (FMath::IsNearlyZero(InModelingTime) || FMath::IsNearlyZero(InCognitiveEnergy))
	{
		UE_LOG(LogTemp, Error, TEXT("Modeling time and cognitive energy must be non-zero."));
		return 0.0f;
	}

	return InKappa * (InDeltaR / (InModelingTime * InCognitiveEnergy));
}

float AGeochemicalReactionModelingIndexActor::GetGRMI() const
{
	return GRMIValue;
}
