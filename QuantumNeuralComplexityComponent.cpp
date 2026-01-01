#include "QuantumNeuralComplexityComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UQuantumNeuralComplexityComponent::UQuantumNeuralComplexityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuantumNeuralComplexityComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentTime = 0.0f;
	CurrentComplexity = InitialComplexity;
}

void UQuantumNeuralComplexityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UpdateInterval <= 0.0f)
	{
		return;
	}

	static float NextUpdateTime = 0.0f;
	const float WorldTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	if (WorldTime < NextUpdateTime)
	{
		return;
	}

	NextUpdateTime = WorldTime + UpdateInterval;
	CurrentTime += UpdateInterval;
	CurrentComplexity = InitialComplexity * FMath::Exp(GrowthRate * CurrentTime);

	UE_LOG(LogTemp, Log, TEXT("Time: %0.2fs, Quantum Neural Complexity: %0.4f"), CurrentTime, CurrentComplexity);
}
