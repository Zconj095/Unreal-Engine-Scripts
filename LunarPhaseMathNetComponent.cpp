#include "LunarPhaseMathNetComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarPhaseMathNetComponent::ULunarPhaseMathNetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULunarPhaseMathNetComponent::BeginPlay()
{
	Super::BeginPlay();
	PhaseAngle = CalculatePhaseWithTrig(SunAngle, MoonAngle);
	UE_LOG(LogTemp, Log, TEXT("Lunar Phase Angle with MathNet: %.4f radians"), PhaseAngle);
}

double ULunarPhaseMathNetComponent::CalculatePhaseWithTrig(double ThetaS, double ThetaM) const
{
	const double DeltaTheta = ThetaS - ThetaM;
	return FMath::Acos(-FMath::Cos(DeltaTheta));
}
