#include "LogicAndSystemArchitectureComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULogicAndSystemArchitectureComponent::ULogicAndSystemArchitectureComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULogicAndSystemArchitectureComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLogicFlowRate();
}

double ULogicAndSystemArchitectureComponent::CalculateLogicFlowRate() const
{
	if (EventProbabilities.Num() != EventTimes.Num())
	{
		return 0.0;
	}

	double TotalRate = 0.0;
	for (int32 Index = 0; Index < EventProbabilities.Num(); ++Index)
	{
		const double Time = EventTimes[Index];
		if (FMath::IsNearlyZero(Time))
		{
			return 0.0;
		}
		TotalRate += EventProbabilities[Index] / Time;
	}

	return TotalRate;
}

void ULogicAndSystemArchitectureComponent::LogLogicFlowRate(double Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Total Logic Flow Rate (L): %.4f"), Value);
	UE_LOG(LogTemp, Log, TEXT("Predicted Logic Flow Rate (mock): %.4f"), PredictLogicFlowRateMock());
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (mock): %.4f"), TensorFlowPlaceholder());
}

double ULogicAndSystemArchitectureComponent::PredictLogicFlowRateMock() const
{
	return LogicFlowRate * 1.03;
}

double ULogicAndSystemArchitectureComponent::TensorFlowPlaceholder() const
{
	return LogicFlowRate;
}

void ULogicAndSystemArchitectureComponent::RecalculateLogicFlowRate()
{
	LogicFlowRate = CalculateLogicFlowRate();
	LogLogicFlowRate(LogicFlowRate);
}
