#include "RenderingAndVisualDynamicsComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

URenderingAndVisualDynamicsComponent::URenderingAndVisualDynamicsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SampleDistances = { 2.0f, 4.0f, 6.0f, 8.0f, 10.0f };
}

void URenderingAndVisualDynamicsComponent::BeginPlay()
{
	Super::BeginPlay();

	Intensity = CalculateIntensity(Luminosity, Distance);
	UE_LOG(LogTemp, Log, TEXT("Light Intensity (I) at distance %.2f: %.4f"), Distance, Intensity);

	SampleIntensityAcrossDistances();
}

float URenderingAndVisualDynamicsComponent::CalculateIntensity(float InLuminosity, float InDistance) const
{
	if (FMath::IsNearlyZero(InDistance))
	{
		UE_LOG(LogTemp, Warning, TEXT("Distance cannot be zero when calculating light intensity."));
		return 0.0f;
	}

	const float Denominator = 4.0f * PI * FMath::Square(InDistance);
	return InLuminosity / Denominator;
}

void URenderingAndVisualDynamicsComponent::SampleIntensityAcrossDistances()
{
	SampleIntensities.Empty();
	for (int32 Index = 0; Index < SampleDistances.Num(); ++Index)
	{
		const float DistanceValue = SampleDistances[Index];
		const float Sample = CalculateIntensity(Luminosity, DistanceValue);
		SampleIntensities.Add(Sample);
		UE_LOG(LogTemp, Log, TEXT("Sample [%d] distance %.2f => intensity %.4f"), Index + 1, DistanceValue, Sample);
	}
}
