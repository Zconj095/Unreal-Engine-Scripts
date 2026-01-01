#include "PerceptualDistortionDetectionComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPerceptualDistortionDetectionComponent::UPerceptualDistortionDetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPerceptualDistortionDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	const TArray<float> Volume = GenerateVolume();
	PerceptualDistortionMagnitude = 0.0f;

	for (float Position : Volume)
	{
		const float Gradient = ApproximateGradient(Position);
		PerceptualDistortionMagnitude += FMath::Square(Gradient);
	}

	UE_LOG(LogTemp, Log, TEXT("Perceptual Distortion Magnitude (P): %0.4f"), PerceptualDistortionMagnitude);
}

float UPerceptualDistortionDetectionComponent::CalculatePerceptualDistortion()
{
	const TArray<float> Volume = GenerateVolume();
	float Total = 0.0f;

	for (float Position : Volume)
	{
		const float Gradient = ApproximateGradient(Position);
		Total += FMath::Square(Gradient);
	}

	PerceptualDistortionMagnitude = Total;
	return Total;
}

TArray<float> UPerceptualDistortionDetectionComponent::GenerateVolume() const
{
	TArray<float> Volume;
	if (VolumeStep <= 0.0f)
	{
		return Volume;
	}

	for (float Position = VolumeStart; Position <= VolumeEnd; Position += VolumeStep)
	{
		Volume.Add(Position);
	}

	return Volume;
}

float UPerceptualDistortionDetectionComponent::EvaluatePsi(float Value) const
{
	return PsiAmplitude * FMath::Sin(Value) + PsiOffset;
}

float UPerceptualDistortionDetectionComponent::ApproximateGradient(float Position) const
{
	const float H = FMath::Max(GradientStep, KINDA_SMALL_NUMBER);
	const float Forward = EvaluatePsi(Position + H);
	const float Backward = EvaluatePsi(Position - H);
	return (Forward - Backward) / (2.0f * H);
}
