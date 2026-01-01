// Copyright Epic Games, Inc.

#include "QuantumPhaseEstimationGateObject.h"

#include "Logging/LogMacros.h"

UQuantumPhaseEstimationGateObject::UQuantumPhaseEstimationGateObject()
{
	Name = TEXT("Quantum Phase Estimation Gate");
}

TArray<float> UQuantumPhaseEstimationGateObject::Apply(const TArray<float>& StateVector) const
{
	if (StateVector.Num() % 2 != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateVector length must be even (interleaved real/imag)."));
		return StateVector;
	}

	const int32 StateSize = StateVector.Num() / 2;
	TArray<float> NewState;
	NewState.SetNum(StateVector.Num());

	for (int32 i = 0; i < StateSize; ++i)
	{
		const int32 RealIndex = i * 2;
		const int32 ImagIndex = RealIndex + 1;

		if ((i & (1 << TargetQubit)) != 0)
		{
			const float RealPart = StateVector[RealIndex];
			const float ImagPart = StateVector[ImagIndex];

			const bool bControlActive = [&]()
			{
				for (int32 c = 0; c < NumControlQubits; ++c)
				{
					if ((i & (1 << c)) == 0)
					{
						return false;
					}
				}
				return true;
			}();

			const float Rotation = bControlActive ? PhaseAngle : 0.f;
			const float CosTheta = FMath::Cos(Rotation);
			const float SinTheta = FMath::Sin(Rotation);

			NewState[RealIndex] = RealPart * CosTheta - ImagPart * SinTheta;
			NewState[ImagIndex] = RealPart * SinTheta + ImagPart * CosTheta;
		}
		else
		{
			NewState[RealIndex] = StateVector[RealIndex];
			NewState[ImagIndex] = StateVector[ImagIndex];
		}
	}

	return NewState;
}
