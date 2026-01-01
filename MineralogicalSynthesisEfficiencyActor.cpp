#include "MineralogicalSynthesisEfficiencyActor.h"
#include "Math/UnrealMathUtility.h"

AMineralogicalSynthesisEfficiencyActor::AMineralogicalSynthesisEfficiencyActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMineralogicalSynthesisEfficiencyActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeMSE();
}

void AMineralogicalSynthesisEfficiencyActor::ComputeMSE() const
{
	if (TotalNeurons <= 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Total neurons must be greater than zero."));
		return;
	}

	const float Ratio = NeuronsMineral / TotalNeurons;
	const float MSE = Zeta * FMath::LogX(2.f, 1.f + Ratio);
	UE_LOG(LogTemp, Log, TEXT("Mineralogical Synthesis Efficiency (MSE): %.4f"), MSE);
}
