#include "LunarPhaseMemoryRetentionComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarPhaseMemoryRetentionComponent::ULunarPhaseMemoryRetentionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarPhaseMemoryRetentionComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentTime = 0.0f;
	MemoryRetention = CalculateMemoryRetention(CurrentTime);
	LogRetention();
}

void ULunarPhaseMemoryRetentionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentTime += DeltaTime;
	MemoryRetention = CalculateMemoryRetention(CurrentTime);
	LogRetention();
}

float ULunarPhaseMemoryRetentionComponent::CalculateMemoryRetention(float Time) const
{
	const float DecayTerm = InitialMemoryCapacity * FMath::Exp(-MemoryDecayRate * Time);
	const float BoostTerm = MemoryBoost * FMath::Exp(-LunarSensitivity * FMath::Square(Time - PeakAlignmentTime));
	return DecayTerm + BoostTerm;
}

void ULunarPhaseMemoryRetentionComponent::LogRetention() const
{
	UE_LOG(LogTemp, Log, TEXT("Memory Retention: %.2f"), MemoryRetention);
}
