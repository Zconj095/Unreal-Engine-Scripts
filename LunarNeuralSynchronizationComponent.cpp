#include "LunarNeuralSynchronizationComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarNeuralSynchronizationComponent::ULunarNeuralSynchronizationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarNeuralSynchronizationComponent::BeginPlay()
{
	Super::BeginPlay();
	SynchronizedNeuralActivity = CalculateNeuralSynchronization(0.0f);
}

void ULunarNeuralSynchronizationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const float Time = GetWorld()->GetTimeSeconds();
	SynchronizedNeuralActivity = CalculateNeuralSynchronization(Time);
	UE_LOG(LogTemp, Log, TEXT("Synchronized Neural Activity: %.4f"), SynchronizedNeuralActivity);
}

float ULunarNeuralSynchronizationComponent::CalculateNeuralSynchronization(float Time) const
{
	return BaselineNeuralActivity * (1.0f + LunarInfluenceAmplitude * FMath::Cos(AngularFrequency * Time + PhaseOffset));
}
