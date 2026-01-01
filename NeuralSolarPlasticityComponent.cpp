#include "NeuralSolarPlasticityComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralSolarPlasticityComponent::UNeuralSolarPlasticityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralSolarPlasticityComponent::BeginPlay()
{
	Super::BeginPlay();

	LogPlasticity();
}

float UNeuralSolarPlasticityComponent::CalculateRadiantFlux(float TimeSeconds) const
{
	return EFluxBase * (1.0f + FMath::Sin(TimeSeconds));
}

float UNeuralSolarPlasticityComponent::IntegratePlasticity(float Duration) const
{
	const int32 Steps = FMath::Clamp(IntegrationSteps, 4, 1000);
	const float Dx = Duration / Steps;
	double Sum = 0.0;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float CurrentTime = Index * Dx;
		const float Flux = CalculateRadiantFlux(CurrentTime);
		Sum += Flux * Dx;
	}

	return static_cast<float>(Mu * Sum);
}

float UNeuralSolarPlasticityComponent::CalculatePlasticity() const
{
	return IntegratePlasticity(ExposureDuration);
}

void UNeuralSolarPlasticityComponent::LogPlasticity() const
{
	const float PlasticityChange = CalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("?N(t) after exposure of %.2fs: %.4f"), ExposureDuration, PlasticityChange);
}
