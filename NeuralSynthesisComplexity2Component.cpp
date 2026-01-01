#include "NeuralSynthesisComplexity2Component.h"
#include "Logging/LogMacros.h"

UNeuralSynthesisComplexity2Component::UNeuralSynthesisComplexity2Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralSynthesisComplexity2Component::BeginPlay()
{
	Super::BeginPlay();

	ComplexityValue = CalculateComplexity(InitialComplexity, GrowthRate, Time);
}

void UNeuralSynthesisComplexity2Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ComplexityValue = CalculateComplexity(InitialComplexity, GrowthRate, Time);
	UE_LOG(LogTemp, Log, TEXT("Complexity at time %0.2f: %0.4f"), Time, ComplexityValue);
	Time += DeltaTime;
}

void UNeuralSynthesisComplexity2Component::SetInitialComplexity(float Value)
{
	InitialComplexity = Value;
}

void UNeuralSynthesisComplexity2Component::SetGrowthRate(float Value)
{
	GrowthRate = Value;
}

void UNeuralSynthesisComplexity2Component::SetTime(float Value)
{
	Time = Value;
}

float UNeuralSynthesisComplexity2Component::CalculateComplexity(float Initial, float Rate, float TimeValue) const
{
	return Initial * FMath::Exp(Rate * TimeValue);
}
