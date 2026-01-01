#include "GeochemicalCycleModelActor.h"

#include "Math/UnrealMathUtility.h"

AGeochemicalCycleModelActor::AGeochemicalCycleModelActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeochemicalCycleModelActor::BeginPlay()
{
	Super::BeginPlay();

	if (FMath::IsNearlyZero(StandardRatio))
	{
		UE_LOG(LogTemp, Error, TEXT("Standard ratio must be non-zero."));
		return;
	}

	TArray<double> Values = SimulateFractionation();
	if (bLogResults)
	{
		for (int32 Index = 0; Index < Values.Num(); ++Index)
		{
			UE_LOG(LogTemp, Display, TEXT("Sample %d: δ13C = %.4f"), Index + 1, Values[Index]);
		}
	}
}

double AGeochemicalCycleModelActor::ComputeIsotopicFractionation(double SampleRatio) const
{
	return ((SampleRatio / StandardRatio) - 1.0) * 1000.0;
}

TArray<double> AGeochemicalCycleModelActor::SimulateFractionation() const
{
	TArray<double> Result;
	Result.Reserve(SampleRatios.Num());
	for (double Ratio : SampleRatios)
	{
		Result.Add(ComputeIsotopicFractionation(Ratio));
	}
	return Result;
}
