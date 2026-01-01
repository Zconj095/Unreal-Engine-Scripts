#include "NeuralMemoryEncodingComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

UNeuralMemoryEncodingComponent::UNeuralMemoryEncodingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralMemoryEncodingComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetEncodingState();
}

void UNeuralMemoryEncodingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetWorld())
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (!bHasPreviousTime)
	{
		PreviousTime = CurrentTime;
		bHasPreviousTime = true;
		return;
	}

	const float Delta = CurrentTime - PreviousTime;
	if (Delta <= 0.0f)
	{
		return;
	}

	const float CurrentComplexity = EvaluateNeuralComplexity(CurrentTime);
	const float PreviousComplexity = EvaluateNeuralComplexity(PreviousTime);

	MemoryCapacity += EncodingEfficiency * (PreviousComplexity + CurrentComplexity) * 0.5f * Delta;
	PreviousTime = CurrentTime;

	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Log, TEXT("Memory Capacity (Mn): %.2f"), MemoryCapacity);
	}
}

void UNeuralMemoryEncodingComponent::ResetEncodingState()
{
	MemoryCapacity = 0.0f;
	PreviousTime = 0.0f;
	bHasPreviousTime = false;
}

float UNeuralMemoryEncodingComponent::EvaluateNeuralComplexity(float Time) const
{
	if (const FRichCurve* Curve = NeuralComplexityOverTime.GetRichCurveConst())
	{
		return Curve->Eval(Time);
	}

	return 0.0f;
}
