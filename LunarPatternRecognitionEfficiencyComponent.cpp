#include "LunarPatternRecognitionEfficiencyComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarPatternRecognitionEfficiencyComponent::ULunarPatternRecognitionEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarPatternRecognitionEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();
	RecognitionEfficiency = CalculateLPRE(0.0f);
	LogEfficiency();
}

void ULunarPatternRecognitionEfficiencyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	RecognitionEfficiency = CalculateLPRE(CurrentTime);
	LogEfficiency();
}

float ULunarPatternRecognitionEfficiencyComponent::CalculateLPRE(float CurrentTime) const
{
	if (EnergyExpended <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Energy expended (ΔE) must be greater than zero."));
		return 0.0f;
	}

	const float Dx = (EndX - StartX) / Steps;
	float IntegralSum = 0.0f;

	for (int32 Index = 0; Index <= Steps; ++Index)
	{
		const float X = StartX + Index * Dx;
		const float Psi = WaveFunction(X, CurrentTime);
		const float Conj = ConjugateWaveFunction(X, CurrentTime);
		IntegralSum += Psi * Conj * Dx;
	}

	return IntegralSum / EnergyExpended;
}

float ULunarPatternRecognitionEfficiencyComponent::WaveFunction(float X, float Time) const
{
	return FMath::Sin(X + Time);
}

float ULunarPatternRecognitionEfficiencyComponent::ConjugateWaveFunction(float X, float Time) const
{
	return FMath::Sin(X + Time);
}

void ULunarPatternRecognitionEfficiencyComponent::LogEfficiency() const
{
	UE_LOG(LogTemp, Log, TEXT("Recognition Efficiency (η): %.2f"), RecognitionEfficiency);
}
