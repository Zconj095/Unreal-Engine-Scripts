#include "NeuralSynthesisComplexityComponent.h"
#include "Logging/LogMacros.h"

UNeuralSynthesisComplexityComponent::UNeuralSynthesisComplexityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralSynthesisComplexityComponent::BeginPlay()
{
	Super::BeginPlay();

	ComplexityAtTime = CalculateComplexity(InitialComplexity, GrowthRate, TimeAccumulator);
}

void UNeuralSynthesisComplexityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeAccumulator += DeltaTime;
	ComplexityAtTime = CalculateComplexity(InitialComplexity, GrowthRate, TimeAccumulator);
	UE_LOG(LogTemp, Log, TEXT("Complexity at time %0.2f: %0.4f"), TimeAccumulator, ComplexityAtTime);
}

void UNeuralSynthesisComplexityComponent::SetInitialComplexity(float Value)
{
	InitialComplexity = Value;
}

void UNeuralSynthesisComplexityComponent::SetGrowthRate(float Value)
{
	GrowthRate = Value;
}

void UNeuralSynthesisComplexityComponent::SetTime(float Value)
{
	TimeAccumulator = Value;
}

float UNeuralSynthesisComplexityComponent::CalculateComplexity(float Initial, float Rate, float Time) const
{
	return Initial * FMath::Exp(Rate * Time);
}
