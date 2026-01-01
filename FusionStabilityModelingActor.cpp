#include "FusionStabilityModelingActor.h"

#include "Math/UnrealMathUtility.h"

AFusionStabilityModelingActor::AFusionStabilityModelingActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFusionStabilityModelingActor::BeginPlay()
{
	Super::BeginPlay();

	float Numerator = 0.0f;
	float Denominator = 0.0f;
	if (!GetStabilityComponents(Numerator, Denominator))
	{
		UE_LOG(LogTemp, Error, TEXT("Compatibility factors and energy contributions must share the same length."));
		return;
	}

	if (Denominator <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Resistance plus magical energy must be greater than zero."));
		return;
	}

	const float FusionStabilityIndex = Numerator / Denominator;
	UE_LOG(LogTemp, Display, TEXT("Fusion Stability Index (Sf): %.4f"), FusionStabilityIndex);

	if (bLogValidation)
	{
		const float Validation = ComputeTensorFlowLikeValidation(Numerator, Denominator);
		UE_LOG(LogTemp, Display, TEXT("[TensorFlow Validation] Fusion Stability Index (Sf): %.4f"), Validation);
	}
}

float AFusionStabilityModelingActor::CalculateFusionStabilityIndex() const
{
	float Numerator = 0.0f;
	float Denominator = 0.0f;
	if (!GetStabilityComponents(Numerator, Denominator) || Denominator <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	return Numerator / Denominator;
}

bool AFusionStabilityModelingActor::GetStabilityComponents(float& OutNumerator, float& OutDenominator) const
{
	if (CompatibilityFactors.Num() != EnergyContributions.Num())
	{
		return false;
	}

	OutNumerator = 0.0f;
	for (int32 Index = 0; Index < CompatibilityFactors.Num(); ++Index)
	{
		OutNumerator += CompatibilityFactors[Index] * EnergyContributions[Index];
	}

	OutDenominator = Resistance + MagicalEnergyContribution;
	return true;
}

float AFusionStabilityModelingActor::ComputeTensorFlowLikeValidation(float StabilityNumerator, float StabilityDenominator) const
{
	if (StabilityDenominator <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	return StabilityNumerator / StabilityDenominator;
}
