#include "GameplayPatternRecognitionEfficiencyActor.h"

#include "Math/UnrealMathUtility.h"

AGameplayPatternRecognitionEfficiencyActor::AGameplayPatternRecognitionEfficiencyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameplayPatternRecognitionEfficiencyActor::BeginPlay()
{
	Super::BeginPlay();
	RecalculateGPRE();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GPRE: %.4f"), GPREValue);
	}
}

void AGameplayPatternRecognitionEfficiencyActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RecalculateGPRE();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GPRE: %.4f"), GPREValue);
	}
}

void AGameplayPatternRecognitionEfficiencyActor::UpdatePhi(float NewPhi)
{
	Phi = NewPhi;
	RecalculateGPRE();
}

void AGameplayPatternRecognitionEfficiencyActor::UpdateFiringRate(float NewFiringRate)
{
	FiringRate = NewFiringRate;
	RecalculateGPRE();
}

void AGameplayPatternRecognitionEfficiencyActor::UpdateOptimizationTime(float NewOptimizationTime)
{
	if (NewOptimizationTime > KINDA_SMALL_NUMBER)
	{
		OptimizationTime = NewOptimizationTime;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Time required for optimization (T) must be greater than zero."));
	}
	RecalculateGPRE();
}

void AGameplayPatternRecognitionEfficiencyActor::UpdateSynapticDensity(float NewSynapticDensity)
{
	if (NewSynapticDensity > KINDA_SMALL_NUMBER)
	{
		SynapticDensity = NewSynapticDensity;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Synaptic density (σ) must be greater than zero."));
	}
	RecalculateGPRE();
}

float AGameplayPatternRecognitionEfficiencyActor::GetGPRE() const
{
	return GPREValue;
}

void AGameplayPatternRecognitionEfficiencyActor::RecalculateGPRE()
{
	if (OptimizationTime <= KINDA_SMALL_NUMBER || SynapticDensity <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Optimization time (T) and synaptic density (σ) must be greater than zero."));
		GPREValue = 0.0f;
		return;
	}

	GPREValue = (Phi * FiringRate) / (OptimizationTime * SynapticDensity);
}
