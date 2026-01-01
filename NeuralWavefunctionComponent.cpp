#include "NeuralWavefunctionComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeuralWavefunctionComponent::UNeuralWavefunctionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralWavefunctionComponent::BeginPlay()
{
	Super::BeginPlay();

	WavefunctionValue = CalculateWavefunction(Position, Time);
	UE_LOG(LogTemp, Log, TEXT("Neural Wavefunction Ψ(%0.2f, %0.2f) = %s"), Position, Time, *WavefunctionValue.ToString());
}

FNeuralWavefunctionComplex UNeuralWavefunctionComponent::CalculateWavefunction(float InPosition, float InTime) const
{
	const float Phase = Wavevector * InPosition - AngularFrequency * InTime;
	const float CosPhase = FMath::Cos(Phase);
	const float SinPhase = FMath::Sin(Phase);
	return FNeuralWavefunctionComplex(Amplitude * CosPhase, Amplitude * SinPhase);
}
