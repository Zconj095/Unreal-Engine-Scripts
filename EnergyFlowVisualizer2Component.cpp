#include "EnergyFlowVisualizer2Component.h"
#include "Components/TextRenderComponent.h"

UEnergyFlowVisualizer2Component::UEnergyFlowVisualizer2Component()
{
	PrimaryComponentTick.bCanEverTick = false;
	TotalEnergy = 100.0;
	Resistances = { 2.0, 3.0, 5.0 };
	DisplayText = nullptr;
}

void UEnergyFlowVisualizer2Component::BeginPlay()
{
	Super::BeginPlay();

	const double Power = CalculatePower(TotalEnergy, Resistances);
	UpdateDisplay(Power);
	UE_LOG(LogTemp, Log, TEXT("Power Allocated: %f"), Power);
}

void UEnergyFlowVisualizer2Component::UpdateDisplay(double Power) const
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(FString::Printf(TEXT("Power Allocated: %.2f"), Power)));
	}
}

double UEnergyFlowVisualizer2Component::CalculatePower(double InTotalEnergy, const TArray<double>& InResistances) const
{
	double TotalResistance = 0.0;
	for (double Resistance : InResistances)
	{
		TotalResistance += Resistance;
	}

	if (FMath::IsNearlyZero(TotalResistance))
	{
		UE_LOG(LogTemp, Warning, TEXT("Total resistance is zero, cannot divide."));
		return 0.0;
	}

	return InTotalEnergy / TotalResistance;
}
