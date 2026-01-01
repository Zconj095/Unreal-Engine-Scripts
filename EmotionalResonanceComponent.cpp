#include "EmotionalResonanceComponent.h"
#include "Math/UnrealMathUtility.h"

UEmotionalResonanceComponent::UEmotionalResonanceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Alpha = {0.5f, 0.3f, 0.2f};
	Intensities = {80.0f, 60.0f, 40.0f};
	NeuralWeight = 0.75f;
	TotalResonance = 0.0f;
}

void UEmotionalResonanceComponent::BeginPlay()
{
	Super::BeginPlay();

	TotalResonance = CalculateEmotionalResonance(Alpha, Intensities);
	UE_LOG(LogTemp, Log, TEXT("Total Emotional Resonance (E_R): %f"), TotalResonance);

	if (Alpha.Num() >= 1 && Intensities.Num() >= 1)
	{
		TArray<float> Input = {Alpha[0], Intensities[0]};
		TArray<float> Prediction = PredictEmotionalResonance(Input);
		if (Prediction.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Predicted Emotional Resonance (E_R): %f"), Prediction[0]);
		}
	}

	float TfResult = ComputeWithTensorFlow(Alpha, Intensities);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (E_R): %f"), TfResult);
}

float UEmotionalResonanceComponent::CalculateEmotionalResonance(const TArray<float>& InAlpha, const TArray<float>& InR) const
{
	float Total = 0.0f;
	const int32 Count = FMath::Min(InAlpha.Num(), InR.Num());

	for (int32 Index = 0; Index < Count; ++Index)
	{
		Total += InAlpha[Index] * InR[Index];
	}

	return Total;
}

TArray<float> UEmotionalResonanceComponent::PredictEmotionalResonance(const TArray<float>& Input) const
{
	if (Input.IsEmpty())
	{
		return {};
	}

	float Sum = 0.0f;
	for (float Value : Input)
	{
		Sum += Value;
	}

	const float Result = FMath::Clamp(FMath::Sin(Sum * NeuralWeight), 0.0f, 1.0f);
	return {Result};
}

float UEmotionalResonanceComponent::ComputeWithTensorFlow(const TArray<float>& InAlpha, const TArray<float>& InR) const
{
	// Placeholder: mimic TensorFlow result using the same calculation.
	return CalculateEmotionalResonance(InAlpha, InR);
}
