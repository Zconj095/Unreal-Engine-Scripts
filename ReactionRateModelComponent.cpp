#include "ReactionRateModelComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UReactionRateModelComponent::UReactionRateModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UReactionRateModelComponent::BeginPlay()
{
	Super::BeginPlay();

	ReactionRates = SimulateReactionRates();
	for (int32 Index = 0; Index < ReactionRates.Num(); ++Index)
	{
		UE_LOG(LogTemp, Log, TEXT("Reaction Rate [%d]: %0.4f"), Index + 1, ReactionRates[Index]);
	}
}

float UReactionRateModelComponent::ComputeReactionRate(float Temperature, float MagicalField) const
{
	if (FMath::IsNearlyZero(GasConstant * Temperature))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid denominator when computing reaction rate."));
		return 0.0f;
	}

	const float Arrhenius = FrequencyFactor * FMath::Exp(-ActivationEnergy / (GasConstant * Temperature));
	const float MagicalEnhancement = Gamma * MagicalField;
	return Arrhenius + MagicalEnhancement;
}

TArray<float> UReactionRateModelComponent::SimulateReactionRates() const
{
	TArray<float> Rates;
	if (Temperatures.Num() != MagicalFields.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Temperature and MagicalField arrays must be equal."));
		return Rates;
	}

	Rates.Reserve(Temperatures.Num());
	for (int32 Index = 0; Index < Temperatures.Num(); ++Index)
	{
		Rates.Add(ComputeReactionRate(Temperatures[Index], MagicalFields[Index]));
	}
	return Rates;
}
