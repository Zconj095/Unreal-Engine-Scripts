#include "MagecraftSystem.h"

#include "Math/UnrealMathUtility.h"
#include <cfloat>

namespace
{
	/** Clamp to a sane gamma to avoid infinities when velocity fraction is near 1. */
	double ComputeLorentzGamma(double VelocityFractionOfC)
	{
		const double Clamped = FMath::Clamp(VelocityFractionOfC, 0.0, 0.999999);
		const double Beta = Clamped;
		return 1.0 / FMath::Sqrt(1.0 - (Beta * Beta));
	}
}

void UMagecraftSystem::HyperdimensionalProcessing(const TArray<float>& Signal, TArray<float>& OutMagnitude)
{
	const int32 N = Signal.Num();
	OutMagnitude.SetNumZeroed(N);
	if (N == 0)
	{
		return;
	}

	const double TwoPi = 2.0 * PI;
	for (int32 K = 0; K < N; ++K)
	{
		double Real = 0.0;
		double Imag = 0.0;
		for (int32 NIdx = 0; NIdx < N; ++NIdx)
		{
			const double Angle = -(TwoPi * static_cast<double>(K) * static_cast<double>(NIdx)) / static_cast<double>(N);
			const double CosA = FMath::Cos(Angle);
			const double SinA = FMath::Sin(Angle);
			const double Sample = static_cast<double>(Signal[NIdx]);
			Real += Sample * CosA;
			Imag += Sample * SinA;
		}

		OutMagnitude[K] = static_cast<float>(FMath::Sqrt((Real * Real) + (Imag * Imag)));
	}
}

void UMagecraftSystem::ApplyCustomKernel(TArray<float>& Data)
{
	for (float& Value : Data)
	{
		Value = FMath::Sin(Value);
	}
}

void UMagecraftSystem::QuantumEntangle(const TArray<FVector>& Points, TArray<float>& OutEntangled, float Gamma)
{
	const int32 Count = Points.Num();
	OutEntangled.SetNumZeroed(Count * Count);
	if (Count == 0)
	{
		return;
	}

	for (int32 Row = 0; Row < Count; ++Row)
	{
		for (int32 Col = 0; Col < Count; ++Col)
		{
			const double DistSq = FVector::DistSquared(Points[Row], Points[Col]);
			const float Value = FMath::Exp(-Gamma * static_cast<float>(DistSq));
			OutEntangled[(Row * Count) + Col] = Value;
		}
	}
}

float UMagecraftSystem::EnchantedNetworkForward(const TArray<float>& Weights, int32 InputDim, int32 HiddenDim, const TArray<float>& Input)
{
	if (InputDim <= 0 || HiddenDim <= 0 || Input.Num() != InputDim)
	{
		return 0.0f;
	}

	const int32 ExpectedWeights = (InputDim * HiddenDim) + HiddenDim; // no biases; last HiddenDim entries are output weights
	if (Weights.Num() < ExpectedWeights)
	{
		return 0.0f;
	}

	TArray<float> Hidden;
	Hidden.SetNumZeroed(HiddenDim);

	int32 WIndex = 0;
	for (int32 H = 0; H < HiddenDim; ++H)
	{
		float Sum = 0.0f;
		for (int32 I = 0; I < InputDim; ++I)
		{
			Sum += Input[I] * Weights[WIndex++];
		}
		Hidden[H] = FMath::Max(0.0f, Sum); // ReLU
	}

	float Output = 0.0f;
	for (int32 H = 0; H < HiddenDim; ++H)
	{
		Output += Hidden[H] * Weights[WIndex++];
	}

	return 1.0f / (1.0f + FMath::Exp(-Output)); // Sigmoid
}

int32 UMagecraftSystem::GroversMagicEnhancedSearch(const TArray<float>& Values)
{
	int32 BestIndex = INDEX_NONE;
	float BestValue = -FLT_MAX;

	for (int32 Index = 0; Index < Values.Num(); ++Index)
	{
		if (Values[Index] > BestValue)
		{
			BestValue = Values[Index];
			BestIndex = Index;
		}
	}

	return BestIndex;
}

float UMagecraftSystem::PhononPhotonInteraction(const TArray<float>& PhononSignal, float PhotonState, float VelocityFractionOfC)
{
	TArray<float> Magnitude;
	HyperdimensionalProcessing(PhononSignal, Magnitude);

	double SumFrequencies = 0.0;
	for (float Value : Magnitude)
	{
		SumFrequencies += static_cast<double>(Value);
	}

	const double Gamma = ComputeLorentzGamma(static_cast<double>(VelocityFractionOfC));
	const double Result = (SumFrequencies * Gamma) + static_cast<double>(PhotonState);
	return static_cast<float>(Result);
}

FQuantumMagicalFramework::FQuantumMagicalFramework(int32 InDimensions)
	: Dimensions(InDimensions)
{
}

TArray<float> FQuantumMagicalFramework::TransformData(const TArray<float>& Data) const
{
	const TArray<float> Entangled = EntangleData(Data);
	return MagicalActivation(Entangled);
}

TArray<float> FQuantumMagicalFramework::QuantumComputation(const TArray<float>& Data) const
{
	return TransformData(Data);
}

TArray<float> FQuantumMagicalFramework::InitializeQuantumState(int32 InDimensions) const
{
	const int32 StateSize = 1 << FMath::Clamp(InDimensions, 0, 20); // cap to avoid runaway allocations
	TArray<float> State;
	State.Init(0.0f, StateSize);

	if (StateSize > 0)
	{
		const int32 EnhancedIndex = FMath::RandRange(0, StateSize - 1);
		State[EnhancedIndex] = 1.0f;
	}

	return State;
}

TArray<float> FQuantumMagicalFramework::EntangleData(const TArray<float>& Data) const
{
	// Use the same DFT magnitude used in HyperdimensionalProcessing.
	TArray<float> Magnitude;
	UMagecraftSystem::HyperdimensionalProcessing(Data, Magnitude);

	// Apply a decay similar to the Python "magical factor".
	TArray<float> Entangled;
	Entangled.SetNumZeroed(Magnitude.Num());
	for (int32 Idx = 0; Idx < Magnitude.Num(); ++Idx)
	{
		const float MagicalFactor = FMath::Exp(-FMath::Square(Magnitude[Idx]));
		Entangled[Idx] = Magnitude[Idx] * MagicalFactor;
	}

	return Entangled;
}

TArray<float> FQuantumMagicalFramework::MagicalActivation(const TArray<float>& Data) const
{
	TArray<float> Activated;
	Activated.SetNumZeroed(Data.Num());

	for (int32 Idx = 0; Idx < Data.Num(); ++Idx)
	{
		// Small random jitter mirrors the Python noise; safe for illustrative purposes.
		const float Noise = FMath::FRandRange(-0.1f, 0.1f);
		Activated[Idx] = FMath::Tanh(Data[Idx]) + Noise;
	}

	return Activated;
}
