#include "EntropyManagementComponent.h"
#include "Math/UnrealMathUtility.h"

UEntropyManagementComponent::UEntropyManagementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	IncrementalHeatTransfer = {10.0, 20.0, 30.0};
	Temperature = {300.0, 310.0, 320.0};
	MagicEnergyGradient = 5.0;
	ChangeInEntropy = 0.0;
}

void UEntropyManagementComponent::BeginPlay()
{
	Super::BeginPlay();

	ChangeInEntropy = CalculateEntropyChange(IncrementalHeatTransfer, Temperature, MagicEnergyGradient);
	UE_LOG(LogTemp, Log, TEXT("Change in Entropy (ΔS): %f"), ChangeInEntropy);
	LogNeuralNetworkPlaceholder();
	LogTensorFlowPlaceholder();
}

double UEntropyManagementComponent::CalculateEntropyChange(const TArray<double>& HeatTransfer, const TArray<double>& Temperatures, double Gradient) const
{
	double EntropyIntegral = 0.0;
	const int32 Count = FMath::Min(HeatTransfer.Num(), Temperatures.Num());
	for (int32 Index = 0; Index < Count; ++Index)
	{
		if (FMath::IsNearlyZero(Temperatures[Index]))
		{
			continue;
		}
		EntropyIntegral += HeatTransfer[Index] / Temperatures[Index];
	}
	return EntropyIntegral + Gradient;
}

void UEntropyManagementComponent::LogNeuralNetworkPlaceholder() const
{
	const double Prediction = ChangeInEntropy * 0.98;
	UE_LOG(LogTemp, Log, TEXT("Predicted Change in Entropy (ΔS): %f"), Prediction);
}

void UEntropyManagementComponent::LogTensorFlowPlaceholder() const
{
	UE_LOG(LogTemp, Log, TEXT("TensorFlowSharp Calculated Change in Entropy (ΔS): %f"), ChangeInEntropy);
}
