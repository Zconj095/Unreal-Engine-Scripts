#include "NeuralPlasticityLunarLearningComponent.h"
#include "Logging/LogMacros.h"

UNeuralPlasticityLunarLearningComponent::UNeuralPlasticityLunarLearningComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralPlasticityLunarLearningComponent::BeginPlay()
{
	Super::BeginPlay();

	SimulateLunarPlasticity();
}

void UNeuralPlasticityLunarLearningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Log, TEXT("Real-Time ?N(t): %0.2f"), AccumulatedNeuralPlasticity);
}

void UNeuralPlasticityLunarLearningComponent::SimulateLunarPlasticity()
{
	while (ElapsedTime < SimulationDuration)
	{
		const float LunarPotential = CalculateLunarPotential(ElapsedTime);
		const float PlasticityIncrement = LunarAdaptationCoefficient * LunarPotential * TimeStep;
		AccumulatedNeuralPlasticity += PlasticityIncrement;

		UE_LOG(LogTemp, Log, TEXT("Time: %0.2fs, ?N(t): %0.2f, ?_lunar(t): %0.2f"), ElapsedTime, AccumulatedNeuralPlasticity, LunarPotential);
		ElapsedTime += TimeStep;
	}
}

float UNeuralPlasticityLunarLearningComponent::CalculateLunarPotential(float Time) const
{
	return ScalingFactor * FMath::Cos(2 * PI * LunarFrequency * Time + PhaseShift);
}
