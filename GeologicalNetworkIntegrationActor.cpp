#include "GeologicalNetworkIntegrationActor.h"

#include "Math/UnrealMathUtility.h"

AGeologicalNetworkIntegrationActor::AGeologicalNetworkIntegrationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeologicalNetworkIntegrationActor::BeginPlay()
{
	Super::BeginPlay();
	InitializeDefaults();
	IntegrationValue = CalculateIntegration();
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Geological Network Integration (Ig): %.4f"), IntegrationValue);
	}
}

float AGeologicalNetworkIntegrationActor::GetIntegrationValue() const
{
	return IntegrationValue;
}

void AGeologicalNetworkIntegrationActor::InitializeDefaults()
{
	if (PathwayWeights.Num() != NumberOfPathways)
	{
		PathwayWeights.SetNum(NumberOfPathways);
		for (int32 Index = 0; Index < NumberOfPathways; ++Index)
		{
			PathwayWeights[Index] = FMath::FRandRange(0.1f, 1.0f);
		}
	}

	if (PathwayInputs.Num() != NumberOfPathways)
	{
		PathwayInputs.SetNum(NumberOfPathways);
		for (int32 Index = 0; Index < NumberOfPathways; ++Index)
		{
			PathwayInputs[Index] = FMath::FRandRange(0.1f, 5.0f);
		}
	}
}

float AGeologicalNetworkIntegrationActor::CalculateIntegration() const
{
	if (PathwayWeights.Num() != NumberOfPathways || PathwayInputs.Num() != NumberOfPathways)
	{
		UE_LOG(LogTemp, Error, TEXT("Pathway weights and inputs must match NumberOfPathways."));
		return 0.0f;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < NumberOfPathways; ++Index)
	{
		Sum += PathwayWeights[Index] * FMath::Loge(1.0f + PathwayInputs[Index]);
	}

	return Sum;
}
