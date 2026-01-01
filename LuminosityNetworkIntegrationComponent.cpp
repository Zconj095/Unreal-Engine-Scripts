#include "LuminosityNetworkIntegrationComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULuminosityNetworkIntegrationComponent::ULuminosityNetworkIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULuminosityNetworkIntegrationComponent::BeginPlay()
{
	Super::BeginPlay();
	LuminosityIntegration = CalculateIntegration();
	LogIntegration();
}

float ULuminosityNetworkIntegrationComponent::CalculateIntegration() const
{
	if (Weights.Num() != Inputs.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Weights and inputs arrays must have the same length."));
		return 0.0f;
	}

	float IntegrationResult = 0.0f;
	for (int32 Index = 0; Index < Weights.Num(); ++Index)
	{
		const float Value = Inputs[Index];
		IntegrationResult += Weights[Index] * FMath::Loge(1.0f + Value);
	}

	return IntegrationResult;
}

void ULuminosityNetworkIntegrationComponent::LogIntegration() const
{
	UE_LOG(LogTemp, Log, TEXT("Luminosity Network Integration (I_l): %.4f"), LuminosityIntegration);
}

void ULuminosityNetworkIntegrationComponent::UpdateData(const TArray<float>& NewWeights, const TArray<float>& NewInputs)
{
	Weights = NewWeights;
	Inputs = NewInputs;
	LuminosityIntegration = CalculateIntegration();
	LogIntegration();
}
