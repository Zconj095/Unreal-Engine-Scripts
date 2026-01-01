#include "EmotionalMemoryEncodingComponent.h"
#include "Math/UnrealMathUtility.h"

UEmotionalMemoryEncodingComponent::UEmotionalMemoryEncodingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	EncodingEfficiency = 1.0f;
	StartTime = 0.0f;
	EndTime = 10.0f;
	IntegrationSteps = 1000;
	EmotionalMemoryCapacity = 0.0f;
}

void UEmotionalMemoryEncodingComponent::BeginPlay()
{
	Super::BeginPlay();

	EmotionalMemoryCapacity = CalculateEmotionalMemoryCapacity(EncodingEfficiency, StartTime, EndTime, IntegrationSteps);
	UE_LOG(LogTemp, Log, TEXT("Emotional Memory Capacity (M_e): %f"), EmotionalMemoryCapacity);
}

float UEmotionalMemoryEncodingComponent::EmotionalIntensity(float Time) const
{
	return FMath::Sin(Time) + 1.0f;
}

float UEmotionalMemoryEncodingComponent::CalculateEmotionalMemoryCapacity(float Alpha, float T0, float T, int32 Steps) const
{
	if (Steps <= 0 || T <= T0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration bounds or steps for EmotionalMemoryEncodingComponent."));
		return 0.0f;
	}

	const float DT = (T - T0) / static_cast<float>(Steps);
	float Sum = 0.0f;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float CurrentTime = T0 + Index * DT;
		const float Intensity = EmotionalIntensity(CurrentTime);
		Sum += Alpha * Intensity * DT;
	}

	return Sum;
}
