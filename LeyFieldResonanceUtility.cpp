#include "LeyFieldResonanceUtility.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

double ULeyFieldResonanceUtility::ComputeLeyFieldEnergy(const FScalarField& PhiFunction, const FVectorField& LeyFieldFunction, const FVector& MinBounds, const FVector& MaxBounds, int32 Resolution)
{
	if (Resolution <= 0)
	{
		return 0.0;
	}

	const double Dx = (MaxBounds.X - MinBounds.X) / Resolution;
	const double Dy = (MaxBounds.Y - MinBounds.Y) / Resolution;
	const double Dz = (MaxBounds.Z - MinBounds.Z) / Resolution;

	const double VolumeElement = Dx * Dy * Dz;
	double Energy = 0.0;

	for (int32 I = 0; I < Resolution; ++I)
	{
		const double X = MinBounds.X + I * Dx;
		for (int32 J = 0; J < Resolution; ++J)
		{
			const double Y = MinBounds.Y + J * Dy;
			for (int32 K = 0; K < Resolution; ++K)
			{
				const double Z = MinBounds.Z + K * Dz;
				const FVector Position(X, Y, Z);
				const double PhiValue = PhiFunction(Position);
				const FVector LeyField = LeyFieldFunction(Position);
				Energy += PhiValue * LeyField.Size() * VolumeElement;
			}
		}
	}

	return Energy;
}

void ULeyFieldResonanceUtility::Test()
{
	const FVector MinBounds = FVector(-10.0f);
	const FVector MaxBounds = FVector(10.0f);

	auto PhiFunction = [](const FVector& Position) -> double
	{
		return FMath::Exp(-Position.Size());
	};

	auto LeyFieldFunction = [](const FVector& Position) -> FVector
	{
		return FVector(FMath::Sin(Position.X), FMath::Cos(Position.Y), FMath::Sin(Position.Z));
	};

	double Energy = ComputeLeyFieldEnergy(PhiFunction, LeyFieldFunction, MinBounds, MaxBounds, 40);
	UE_LOG(LogTemp, Log, TEXT("Ley Field Resonance Energy: %.6f"), Energy);
}
