#include "INLBinaryWeightedCircuitComponent.h"
#include "Math/UnrealMathUtility.h"

UINLBinaryWeightedCircuitComponent::UINLBinaryWeightedCircuitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UINLBinaryWeightedCircuitComponent::BeginPlay()
{
	Super::BeginPlay();

	CalculateOutputs();
	CalculateINL();
	LogResults();
}

void UINLBinaryWeightedCircuitComponent::CalculateOutputs()
{
	const int32 MaxDigitalValue = (1 << Resolution) - 1;
	IdealOutputs.Reset();
	MeasuredOutputs.Reset();

	for (int32 DigitalValue = 0; DigitalValue <= MaxDigitalValue; ++DigitalValue)
	{
		const float IdealOutput = VRef * DigitalValue / MaxDigitalValue;
		float MeasuredOutput = IdealOutput + FMath::FRandRange(-NoiseLevel, NoiseLevel);
		MeasuredOutput = FMath::Lerp(TargetRangeMin, TargetRangeMax, MeasuredOutput / VRef);

		IdealOutputs.Add(IdealOutput);
		MeasuredOutputs.Add(MeasuredOutput);
	}
}

void UINLBinaryWeightedCircuitComponent::CalculateINL()
{
	INLValues.Reset();
	const int32 Count = FMath::Min(IdealOutputs.Num(), MeasuredOutputs.Num());
	for (int32 Index = 0; Index < Count; ++Index)
	{
		INLValues.Add(MeasuredOutputs[Index] - IdealOutputs[Index]);
	}
}

void UINLBinaryWeightedCircuitComponent::LogResults() const
{
	UE_LOG(LogTemp, Log, TEXT("Digital Input\tIdeal Output\tMeasured Output\tINL"));
	const int32 Count = FMath::Min(IdealOutputs.Num(), INLValues.Num());
	for (int32 Index = 0; Index < Count; ++Index)
	{
		UE_LOG(LogTemp, Log, TEXT("%d\t\t%.3f\t\t%.3f\t\t%.3f"), Index, IdealOutputs[Index], MeasuredOutputs[Index], INLValues[Index]);
	}
}
