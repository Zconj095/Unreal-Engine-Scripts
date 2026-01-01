#include "HypervectorBoltzmannLibrary.h"
#include "Math/UnrealMathUtility.h"

float UHypervectorBoltzmannLibrary::CalculateProbability(float Energy)
{
	return FMath::Exp(-Energy);
}
