#include "NeurotransmitterDynamicsComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeurotransmitterDynamicsComponent::UNeurotransmitterDynamicsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeurotransmitterDynamicsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNeurotransmitterDynamicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FMath::IsNearlyZero(Sigma) || FMath::IsNearlyZero(Phi))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sigma (σ) and Phi (φ) must be non-zero to avoid division by zero errors."));
		return;
	}

	const float Denominator = FMath::Square(Sigma) + FMath::Square(Phi);
	NeurotransmitterPrecision = (Eta * SynapticStrength) / Denominator;
	UE_LOG(LogTemp, Log, TEXT("Neurotransmitter Precision (D_n): %0.4f"), NeurotransmitterPrecision);
}
