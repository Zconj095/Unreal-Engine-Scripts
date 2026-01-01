#include "MutationRateCalculatorActor.h"

AMutationRateCalculatorActor::AMutationRateCalculatorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMutationRateCalculatorActor::BeginPlay()
{
	Super::BeginPlay();

	const double MutationRate = CalculateMutationRate(Lambda, EnergyBarrier, Temperature, MagicalFactor);
	UE_LOG(LogTemp, Log, TEXT("Mutation Rate: %.6f"), MutationRate);
}

double AMutationRateCalculatorActor::CalculateMutationRate(float InLambda, float InEnergyBarrier, float InTemperature, float InMagicalFactor) const
{
	constexpr double BoltzmannConstant = 1.380649e-23;
	const double ExponentialPart = FMath::Exp(-InEnergyBarrier / (BoltzmannConstant * InTemperature));
	return InLambda * ExponentialPart + InMagicalFactor;
}
