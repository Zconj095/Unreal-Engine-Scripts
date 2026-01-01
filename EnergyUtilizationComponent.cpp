#include "EnergyUtilizationComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyUtilizationComponent::UEnergyUtilizationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	NeuralDensity = 1.0f;
	GridResolution = 10;
	EnergyUtilizationResult = 0.0;
}

void UEnergyUtilizationComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializePotentialField();
}

void UEnergyUtilizationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PotentialField.Num() == 0)
	{
		return;
	}

	EnergyUtilizationResult = ComputeEnergyUtilization();
	UE_LOG(LogTemp, Log, TEXT("Energy Utilization (E_b): %f"), EnergyUtilizationResult);
}

void UEnergyUtilizationComponent::InitializePotentialField()
{
	if (GridResolution < 3)
	{
		GridResolution = 3;
	}

	PotentialField.SetNum(GridResolution * GridResolution);
	for (int32 I = 0; I < GridResolution; ++I)
	{
		for (int32 J = 0; J < GridResolution; ++J)
		{
			const float X = static_cast<float>(I) / static_cast<float>(GridResolution);
			const float Y = static_cast<float>(J) / static_cast<float>(GridResolution);
			PotentialField[I * GridResolution + J] = FMath::Sin(X * PI) * FMath::Cos(Y * PI);
		}
	}
}

double UEnergyUtilizationComponent::ComputeEnergyUtilization() const
{
	if (GridResolution < 3)
	{
		return 0.0;
	}

	const double StepX = 1.0 / GridResolution;
	const double StepY = 1.0 / GridResolution;
	double EnergyUtilization = 0.0;

	for (int32 I = 1; I < GridResolution - 1; ++I)
	{
		for (int32 J = 1; J < GridResolution - 1; ++J)
		{
			const double Laplacian =
				(GetFieldValue(I + 1, J) - 2.0 * GetFieldValue(I, J) + GetFieldValue(I - 1, J)) / (StepX * StepX) +
				(GetFieldValue(I, J + 1) - 2.0 * GetFieldValue(I, J) + GetFieldValue(I, J - 1)) / (StepY * StepY);

			EnergyUtilization += NeuralDensity * Laplacian * StepX * StepY;
		}
	}

	return EnergyUtilization;
}

float UEnergyUtilizationComponent::GetFieldValue(int32 X, int32 Y) const
{
	if (!PotentialField.IsValidIndex(X * GridResolution + Y))
	{
		return 0.0f;
	}
	return PotentialField[X * GridResolution + Y];
}
