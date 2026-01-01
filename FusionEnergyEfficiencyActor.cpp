#include "FusionEnergyEfficiencyActor.h"

#include "Math/UnrealMathUtility.h"

AFusionEnergyEfficiencyActor::AFusionEnergyEfficiencyActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFusionEnergyEfficiencyActor::BeginPlay()
{
	Super::BeginPlay();

	const float Efficiency = CalculateFusionEfficiency(PowerFusion, PowerInput, MagicalEnhancement);
	UE_LOG(LogTemp, Display, TEXT("Fusion Efficiency (η): %.4f"), Efficiency);

	if (bLogValidation)
	{
		const float Validation = ValidateWithTensorFlowLike(PowerFusion, PowerInput, MagicalEnhancement);
		UE_LOG(LogTemp, Display, TEXT("[TensorFlow Validation] Fusion Efficiency (η): %.4f"), Validation);
	}
}

float AFusionEnergyEfficiencyActor::CalculateFusionEfficiency(float FusionPower, float InputPower, float MagicalEnergy) const
{
	const float Denominator = InputPower + MagicalEnergy;
	if (Denominator <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Input power and magical enhancement must sum to a positive value."));
		return 0.0f;
	}

	return FusionPower / Denominator;
}

float AFusionEnergyEfficiencyActor::ValidateWithTensorFlowLike(float FusionPower, float InputPower, float MagicalEnergy) const
{
	// Mirror the same arithmetic that the Unity TensorFlow validation performed.
	return CalculateFusionEfficiency(FusionPower, InputPower, MagicalEnergy);
}
