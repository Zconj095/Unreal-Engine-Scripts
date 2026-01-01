#include "InterstellarNeuralHarmonyComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

UInterstellarNeuralHarmonyComponent::UInterstellarNeuralHarmonyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInterstellarNeuralHarmonyComponent::BeginPlay()
{
	Super::BeginPlay();
	ElapsedTime = 0.0f;
}

void UInterstellarNeuralHarmonyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;
	const float HarmonyValue = CalculateNeuralHarmony(ElapsedTime);
	LogHarmony(HarmonyValue, ElapsedTime);
}

float UInterstellarNeuralHarmonyComponent::CalculateNeuralHarmony(float Time) const
{
	return BaselineHarmony * (1.0f + Gamma * FMath::Sin(Omega * Time + Phi));
}

void UInterstellarNeuralHarmonyComponent::LogHarmony(float HarmonyValue, float TimeValue) const
{
	UE_LOG(LogTemp, Log, TEXT("Neural Harmony at time %.2f: %.4f"), TimeValue, HarmonyValue);
}

float UInterstellarNeuralHarmonyComponent::GetCurrentHarmony() const
{
	return CalculateNeuralHarmony(ElapsedTime);
}
