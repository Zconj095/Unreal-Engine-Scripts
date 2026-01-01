#include "LiftForceComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULiftForceComponent::ULiftForceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULiftForceComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLiftForce();
}

double ULiftForceComponent::ComputeLiftForce() const
{
	return LiftCoefficient * (0.5 * AirDensity * FMath::Square(AirflowVelocity)) * WingArea + MagicalEnergyFlux;
}

double ULiftForceComponent::PredictLiftForceMock() const
{
	return ComputeLiftForce() * 1.02;
}

void ULiftForceComponent::LogResults(double Force) const
{
	UE_LOG(LogTemp, Log, TEXT("Lift Force (L): %.2f N"), Force);
	UE_LOG(LogTemp, Log, TEXT("Predicted Lift Force (mock): %.2f N"), PredictLiftForceMock());
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Calculated Lift Force (mock): %.2f N"), Force);
}

void ULiftForceComponent::RecalculateLiftForce()
{
	LiftForce = ComputeLiftForce();
	LogResults(LiftForce);
}
