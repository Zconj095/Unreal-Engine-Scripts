#include "EnergyFlowOptimization4Component.h"
#include "DrawDebugHelpers.h"

UEnergyFlowOptimization4Component::UEnergyFlowOptimization4Component()
{
	PrimaryComponentTick.bCanEverTick = false;
	EnergyOutput = 100.0f;
	EnergyInput = 200.0f;
	MagicalContribution = 50.0f;
	bDrawVisualization = true;
}

void UEnergyFlowOptimization4Component::BeginPlay()
{
	Super::BeginPlay();
	
	const double Efficiency = CalculateEnergyEfficiency(EnergyOutput, EnergyInput, MagicalContribution);
	UE_LOG(LogTemp, Log, TEXT("Energy Efficiency (η): %f"), Efficiency);
	TensorFlowValidation(EnergyOutput, EnergyInput, MagicalContribution);
	if (bDrawVisualization)
	{
		DrawEfficiencyVisualization();
	}
}

float UEnergyFlowOptimization4Component::CalculateEnergyEfficiency(float Output, float Input, float MuPhi) const
{
	const float Denominator = Input + MuPhi;
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Error, TEXT("Denominator is zero, energy efficiency cannot be calculated!"));
		return 0.0f;
	}
	return Output / Denominator;
}

void UEnergyFlowOptimization4Component::TensorFlowValidation(float Output, float Input, float MuPhi) const
{
	const double Result = CalculateEnergyEfficiency(Output, Input, MuPhi);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Validation - Energy Efficiency: %f"), Result);
}

void UEnergyFlowOptimization4Component::DrawEfficiencyVisualization() const
{
	if (!GetWorld())
	{
		return;
	}

	for (float EInput = 50.0f; EInput <= 500.0f; EInput += 50.0f)
	{
		for (float MuPhi = 10.0f; MuPhi <= 100.0f; MuPhi += 10.0f)
		{
			const float Efficiency = CalculateEnergyEfficiency(EnergyOutput, EInput, MuPhi);
			const FVector Position(EInput / 10.0f, MuPhi / 10.0f, Efficiency);
			DrawDebugSphere(GetWorld(), Position, 5.0f, 12, FColor::Blue, false, 5.0f);
		}
	}
}
