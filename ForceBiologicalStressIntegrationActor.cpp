#include "ForceBiologicalStressIntegrationActor.h"

#include "DrawDebugHelpers.h"

AForceBiologicalStressIntegrationActor::AForceBiologicalStressIntegrationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AForceBiologicalStressIntegrationActor::BeginPlay()
{
	Super::BeginPlay();

	double Stress = CalculateStress(Force, Area, MuPhi);
	LogStress(Stress);
	ValidateStress(Stress, Force, Area, MuPhi);
}

void AForceBiologicalStressIntegrationActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!bDrawDebugSphere)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), DebugSphereRadius, 16, FColor::Red, false, -1.0f, 0, 1.0f);
}

double AForceBiologicalStressIntegrationActor::CalculateStress(double InForce, double InArea, double InMuPhi) const
{
	if (InArea <= 0.0)
	{
		UE_LOG(LogTemp, Error, TEXT("Area must be greater than zero to calculate stress."));
		return 0.0;
	}

	return (InForce / InArea) + InMuPhi;
}

void AForceBiologicalStressIntegrationActor::LogStress(double Stress) const
{
	UE_LOG(LogTemp, Display, TEXT("Stress (σ): %.4f"), Stress);
}

void AForceBiologicalStressIntegrationActor::ValidateStress(double Stress, double InForce, double InArea, double InMuPhi) const
{
	double Expected = CalculateStress(InForce, InArea, InMuPhi);
	if (FMath::Abs(Stress - Expected) > KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("TensorFlow-like validation mismatch: expected %.4f but got %.4f"), Expected, Stress);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("TensorFlow-like validation passed: %.4f"), Stress);
	}
}
