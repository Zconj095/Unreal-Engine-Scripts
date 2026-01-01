#include "GeneticInformationFlowActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticInformationFlowActor::AGeneticInformationFlowActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticInformationFlowActor::BeginPlay()
{
	Super::BeginPlay();

	if (ScalingCoefficients.Num() != GeneticInputs.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("ScalingCoefficients and GeneticInputs must be the same length."));
		return;
	}

	GeneticInformationFlow = CalculateGeneticInformationFlow();
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Genetic Information Flow (I_g): %.4f"), GeneticInformationFlow);
	}
}

float AGeneticInformationFlowActor::CalculateGeneticInformationFlow() const
{
	if (BiasTerm <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Bias term must be greater than zero."));
		return 0.0f;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < ScalingCoefficients.Num(); ++Index)
	{
		Sum += ScalingCoefficients[Index] * FMath::Loge(1.0f + GeneticInputs[Index] / BiasTerm);
	}

	return Sum;
}
