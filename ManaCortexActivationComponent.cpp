#include "ManaCortexActivationComponent.h"
#include "Math/UnrealMathUtility.h"

UManaCortexActivationComponent::UManaCortexActivationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UManaCortexActivationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UManaCortexActivationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float Time = GetWorld()->GetTimeSeconds();
	const float InputSignal = FMath::Sin(Time);
	const float ManaEnergy = ManaBase + ManaVariation * FMath::Sin(2 * PI * ManaFrequency * Time);
	const float Activation = 1.f / (1.f + FMath::Exp(-(InputSignal + ManaEnergy)));

	UE_LOG(LogTemp, Log, TEXT("Input x: %.4f, Mana M: %.4f, Activation f: %.4f"), InputSignal, ManaEnergy, Activation);
}
