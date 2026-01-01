#include "LunarPhasePredictionComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarPhasePredictionComponent::ULunarPhasePredictionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULunarPhasePredictionComponent::BeginPlay()
{
	Super::BeginPlay();
	PhaseAngle = CalculatePhaseAngle(SunAngle, MoonAngle);
	UE_LOG(LogTemp, Log, TEXT("Lunar Phase Angle (phi): %.4f radians"), PhaseAngle);
}

double ULunarPhasePredictionComponent::GetPhaseAngle(double ThetaS, double ThetaM) const
{
	return CalculatePhaseAngle(ThetaS, ThetaM);
}

double ULunarPhasePredictionComponent::CalculatePhaseAngle(double ThetaS, double ThetaM) const
{
	const double DeltaTheta = ThetaS - ThetaM;
	return FMath::Acos(-FMath::Cos(DeltaTheta));
}
