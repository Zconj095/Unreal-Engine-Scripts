#include "MemoryHelixInformationActor.h"
#include "Math/UnrealMathUtility.h"

AMemoryHelixInformationActor::AMemoryHelixInformationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMemoryHelixInformationActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeInformationDensity();
}

double DensityFunction(double X)
{
	return FMath::Exp(-X * X);
}

void AMemoryHelixInformationActor::ComputeInformationDensity() const
{
	const double Step = (UpperBound - LowerBound) / Steps;
	double Integral = 0.0;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const double X = LowerBound + Index * Step;
		const double Density = DensityFunction(X);
		const double Integrand = Density * FMath::Loge(Density / Rho0);
		Integral += Integrand * Step;
	}

	UE_LOG(LogTemp, Log, TEXT("Memory Helix Information Density: %.6f"), Integral);
}
