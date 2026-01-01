#include "LunarNeuralPlasticityComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarNeuralPlasticityComponent::ULunarNeuralPlasticityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarNeuralPlasticityComponent::BeginPlay()
{
	Super::BeginPlay();
	ElapsedTime = 0.0f;
	NeuralPlasticityChange = 0.0f;
}

void ULunarNeuralPlasticityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (ElapsedTime >= SimulationDuration)
	{
		return;
	}

	const float Step = FMath::Min(TimeStep, SimulationDuration - ElapsedTime);
	SimulateStep(Step);
}

float ULunarNeuralPlasticityComponent::CalculateLunarPotential(float Time) const
{
	return ScalingFactor * FMath::Cos(2.0f * PI * LunarFrequency * Time + PhaseShift);
}

void ULunarNeuralPlasticityComponent::SimulateStep(float Step)
{
	const float LunarPotential = CalculateLunarPotential(ElapsedTime);
	const float PlasticityIncrement = LunarResonanceFactor * LunarPotential * Step;
	NeuralPlasticityChange += PlasticityIncrement;
	ElapsedTime += Step;
	LogPlasticity();
}

void ULunarNeuralPlasticityComponent::LogPlasticity() const
{
	UE_LOG(LogTemp, Log, TEXT("Time: %.2fs, ?N(t): %.2f, ?_lunar(t): %.2f"), ElapsedTime, NeuralPlasticityChange, CalculateLunarPotential(ElapsedTime));
}
