#include "EnergyFlowOptimization5Component.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

UEnergyFlowOptimization5Component::UEnergyFlowOptimization5Component()
{
	PrimaryComponentTick.bCanEverTick = false;
	PowerOut = 100.0;
	PowerIn = 200.0;
	MuPhi = 0.5;
	bDrawGizmos = true;
}

void UEnergyFlowOptimization5Component::BeginPlay()
{
	Super::BeginPlay();

	const double Efficiency = CalculateEfficiency(PowerOut, PowerIn, MuPhi);
	UE_LOG(LogTemp, Log, TEXT("Energy Efficiency (η): %f"), Efficiency);
	TensorFlowValidation(PowerOut, PowerIn, MuPhi);
	if (bDrawGizmos && GetWorld())
	{
		const FVector Position(static_cast<float>(PowerOut), static_cast<float>(PowerIn), static_cast<float>(MuPhi));
		DrawDebugSphere(GetWorld(), Position, 30.0f, 12, FColor::Green, true, 10.0f);
	}
}

double UEnergyFlowOptimization5Component::CalculateEfficiency(double Out, double In, double Mu) const
{
	const double Denominator = In + Mu;
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Error, TEXT("Denominator (P_in + μΦ) cannot be zero."));
		return 0.0;
	}
	return Out / Denominator;
}

void UEnergyFlowOptimization5Component::TensorFlowValidation(double Out, double In, double Mu) const
{
	const double Result = CalculateEfficiency(Out, In, Mu);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Validation - Energy Efficiency (η): %f"), Result);
}
