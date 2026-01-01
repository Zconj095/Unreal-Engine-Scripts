#include "MagicalCircuitOptimizationComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

UMagicalCircuitOptimizationComponent::UMagicalCircuitOptimizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMagicalCircuitOptimizationComponent::BeginPlay()
{
	Super::BeginPlay();
const float Omega = ComputeAngularFrequency();
const UMagicalCircuitOptimizationComponent::FComplex Flux = ComputeMagicalFlux(0.01f, Omega);
	FluxText = FString::Printf(TEXT("Magical Flux at t=0.01s, f=%.1fHz: Real=%.4f Imag=%.4f"), Frequency, Flux.real(), Flux.imag());
	UE_LOG(LogTemp, Log, TEXT("%s"), *FluxText);
}

float UMagicalCircuitOptimizationComponent::ComputeAngularFrequency() const
{
	return 2.0f * PI * Frequency;
}

UMagicalCircuitOptimizationComponent::FComplex UMagicalCircuitOptimizationComponent::ComputeImpedance(float Omega) const
{
	const FComplex ImagPart(0.0f, -1.0f / (Omega * Capacitance));
	return FComplex(Resistance, 0.0f) + ImagPart;
}

UMagicalCircuitOptimizationComponent::FComplex UMagicalCircuitOptimizationComponent::ComputeMagicalFlux(float Time, float Omega) const
{
	const float Voltage = FMath::Sin(2.0f * PI * Frequency * Time);
	const FComplex Impedance = ComputeImpedance(Omega);
	return FComplex(Voltage, 0.0f) / Impedance;
}
