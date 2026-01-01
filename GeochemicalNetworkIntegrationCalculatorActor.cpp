#include "GeochemicalNetworkIntegrationCalculatorActor.h"

#include "Math/UnrealMathUtility.h"

AGeochemicalNetworkIntegrationCalculatorActor::AGeochemicalNetworkIntegrationCalculatorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeochemicalNetworkIntegrationCalculatorActor::BeginPlay()
{
	Super::BeginPlay();

	IntegrationResult = CalculateGeochemicalIntegration();
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Geochemical Network Integration (I_g): %.4f"), IntegrationResult);
	}
}

float AGeochemicalNetworkIntegrationCalculatorActor::CalculateGeochemicalIntegration() const
{
	if (PathwayWeights.Num() != InputData.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Pathway weights and input data must have the same length."));
		return 0.0f;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < PathwayWeights.Num(); ++Index)
	{
		Sum += PathwayWeights[Index] * FMath::Loge(1.0f + InputData[Index]);
	}

	return Sum;
}

float AGeochemicalNetworkIntegrationCalculatorActor::GetIntegrationResult() const
{
	return IntegrationResult;
}
