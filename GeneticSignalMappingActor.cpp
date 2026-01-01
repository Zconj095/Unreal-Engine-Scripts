#include "GeneticSignalMappingActor.h"

#include "Math/UnrealMathUtility.h"
#include <cmath>

AGeneticSignalMappingActor::AGeneticSignalMappingActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticSignalMappingActor::BeginPlay()
{
	Super::BeginPlay();

	if (Amplitudes.Num() != Frequencies.Num() || Frequencies.Num() != PhaseShifts.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Signal arrays must share the same length."));
		return;
	}

	const double Signal = CalculateSignal(Position);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Genetic Signal at x=%.4f: %.4f"), Position, Signal);
	}
}

double AGeneticSignalMappingActor::CalculateSignal(double X) const
{
	double Result = 0.0;
	for (int32 Index = 0; Index < Amplitudes.Num(); ++Index)
	{
		const double Term = 2.0 * PI_VALUE * Frequencies[Index] * X + PhaseShifts[Index];
		Result += Amplitudes[Index] * std::sin(Term);
	}

	return Result;
}
