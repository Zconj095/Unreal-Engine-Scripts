#include "HardwareDesignSimulationActor.h"

#include "Math/UnrealMathUtility.h"
#include <complex>

AHardwareDesignSimulationActor::AHardwareDesignSimulationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHardwareDesignSimulationActor::BeginPlay()
{
	Super::BeginPlay();

	const float Step = GetStepSize();
	SimulationData.SetNum(GridDimension * GridDimension * GridDimension);

	for (int32 X = 0; X < GridDimension; ++X)
	{
		for (int32 Y = 0; Y < GridDimension; ++Y)
		{
			for (int32 Z = 0; Z < GridDimension; ++Z)
			{
				const double PosX = -1.0 + X * Step;
				const double PosY = -1.0 + Y * Step;
				const double PosZ = -1.0 + Z * Step;
				const int32 Index = X * GridDimension * GridDimension + Y * GridDimension + Z;
				SimulationData[Index] = EvaluateComplexSum(PosX, PosY, PosZ);
			}
		}
	}

	if (SliceToLog >= 0 && SliceToLog < GridDimension)
	{
		FString Line;
		for (int32 X = 0; X < GridDimension; ++X)
		{
			for (int32 Y = 0; Y < GridDimension; ++Y)
			{
				const int32 Index = X * GridDimension * GridDimension + Y * GridDimension + SliceToLog;
				Line += FString::Printf(TEXT("%.2f "), SimulationData[Index]);
			}
			Line += TEXT("\n");
		}
		UE_LOG(LogTemp, Display, TEXT("Projection (z=%d slice):\n%s"), SliceToLog, *Line);
	}
}

float AHardwareDesignSimulationActor::EvaluateComplexSum(float X, float Y, float Z) const
{
	const double Step = GetStepSize();
	const double Hbar = 1.0545718e-34;
	std::complex<double> Sum(0.0, 0.0);

	for (int32 Component = 0; Component < NumEigenComponents; ++Component)
	{
		const double Sigma = SigmaBase + Component * 0.1;
		const double Psi = FMath::Exp(-((X * X + Y * Y + Z * Z) / (2.0 * Sigma * Sigma)));
		const double Energy = 1.0 + Component * EnergySpacing;
		const double PhaseAngle = -Energy * Time / Hbar;
		const double CosPhase = static_cast<double>(FMath::Cos(static_cast<float>(PhaseAngle)));
		const double SinPhase = static_cast<double>(FMath::Sin(static_cast<float>(PhaseAngle)));
		const std::complex<double> Phase(CosPhase, SinPhase);
		Sum += std::complex<double>(Psi, 0.0) * Phase;
	}

	return static_cast<float>(std::abs(Sum));
}

float AHardwareDesignSimulationActor::GetStepSize() const
{
	if (GridDimension <= 1)
	{
		return 0.0f;
	}
	return 2.0f / (GridDimension - 1);
}

TArray<float> AHardwareDesignSimulationActor::GetSlice(int32 SliceIndex) const
{
	TArray<float> Slice;
	if (SliceIndex < 0 || SliceIndex >= GridDimension)
	{
		return Slice;
	}

	Slice.SetNum(GridDimension * GridDimension);
	for (int32 X = 0; X < GridDimension; ++X)
	{
		for (int32 Y = 0; Y < GridDimension; ++Y)
		{
			const int32 Index = X * GridDimension * GridDimension + Y * GridDimension + SliceIndex;
			Slice[X * GridDimension + Y] = SimulationData.IsValidIndex(Index) ? SimulationData[Index] : 0.0f;
		}
	}
	return Slice;
}
