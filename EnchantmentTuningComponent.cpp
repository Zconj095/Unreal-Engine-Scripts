#include "EnchantmentTuningComponent.h"
#include "Math/UnrealMathUtility.h"

UEnchantmentTuningComponent::UEnchantmentTuningComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MagicFocusCoefficient = 1.0f;
	HarmonicCount = 5;
	TimeStep = 0.1f;
	SimulationDuration = 10.0f;
	RealTimeElapsed = 0.0f;
}

void UEnchantmentTuningComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeParameters();
	SimulateEnchantmentTuning();
}

void UEnchantmentTuningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RealTimeElapsed += DeltaTime;
	const float RealTimePotential = CalculateEnchantmentPotential(RealTimeElapsed);
	UE_LOG(LogTemp, Log, TEXT("Real-Time E_lunar: %f"), RealTimePotential);
}

void UEnchantmentTuningComponent::InitializeParameters()
{
	Amplitudes.SetNum(HarmonicCount);
	Frequencies.SetNum(HarmonicCount);
	PhaseOffsets.SetNum(HarmonicCount);

	for (int32 Index = 0; Index < HarmonicCount; ++Index)
	{
		Amplitudes[Index] = FMath::FRandRange(0.5f, 2.0f);
		Frequencies[Index] = FMath::FRandRange(0.01f, 0.05f);
		PhaseOffsets[Index] = FMath::FRandRange(0.0f, 2.0f * PI);
	}
}

void UEnchantmentTuningComponent::SimulateEnchantmentTuning()
{
	float SimTime = 0.0f;
	while (SimTime < SimulationDuration)
	{
		const float EnchantmentPotential = CalculateEnchantmentPotential(SimTime);
		UE_LOG(LogTemp, Log, TEXT("Time: %.2fs, E_lunar: %.2f"), SimTime, EnchantmentPotential);
		SimTime += TimeStep;
	}
}

float UEnchantmentTuningComponent::CalculateEnchantmentPotential(float Time) const
{
	float RealPart = 0.0f;
	float ImagPart = 0.0f;

	for (int32 Index = 0; Index < HarmonicCount; ++Index)
	{
		const float Phase = 2.0f * PI * Frequencies[Index] * Time + PhaseOffsets[Index];
		const float ContributorReal = Amplitudes[Index] * FMath::Cos(Phase);
		const float ContributorImag = Amplitudes[Index] * FMath::Sin(Phase);
		RealPart += ContributorReal;
		ImagPart += ContributorImag;
	}

	const float Magnitude = FMath::Sqrt(RealPart * RealPart + ImagPart * ImagPart);
	return Magnitude * MagicFocusCoefficient;
}
