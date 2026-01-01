#include "GradientAscentComponent.h"

#include "Logging/LogMacros.h"

UGradientAscentComponent::UGradientAscentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGradientAscentComponent::BeginPlay()
{
	Super::BeginPlay();

	const float MaxPoint = PerformGradientAscent();
	const float MaxValue = Objective(MaxPoint);

	UE_LOG(LogTemp, Log, TEXT("The point of maximum value is: %f"), MaxPoint);
	UE_LOG(LogTemp, Log, TEXT("The maximum value is: %f"), MaxValue);
}

float UGradientAscentComponent::Objective(float X) const
{
	// f(x) = -(a*x^2 - b*x + c) to mirror Unity sample
	return -(CoeffA * X * X - CoeffB * X + CoeffC);
}

float UGradientAscentComponent::ComputeGradient(float X) const
{
	const float H = 0.0001f;
	return (Objective(X + H) - Objective(X)) / H;
}

float UGradientAscentComponent::PerformGradientAscent()
{
	float Current = StartingPoint;

	for (int32 i = 0; i < Iterations; ++i)
	{
		const float Grad = ComputeGradient(Current);
		Current += LearningRate * Grad;

		UE_LOG(LogTemp, Log, TEXT("Iteration %d: x = %.4f, f(x) = %.4f"), i + 1, Current, Objective(Current));
	}

	return Current;
}
