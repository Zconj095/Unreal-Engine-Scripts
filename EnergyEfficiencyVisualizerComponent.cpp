#include "EnergyEfficiencyVisualizerComponent.h"
#include "EnergyAbsorptionStorageComponent.h"
#include "Components/TextRenderComponent.h"

UEnergyEfficiencyVisualizerComponent::UEnergyEfficiencyVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	EnergyStored = 1e6;
	EnergyInput = 2e6;
	DisplayText = nullptr;
}

void UEnergyEfficiencyVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	const double Efficiency = UEnergyAbsorptionStorageComponent::CalculateEfficiency(EnergyStored, EnergyInput);
	UpdateDisplay(Efficiency);
	UE_LOG(LogTemp, Log, TEXT("Energy Storage Efficiency: %.2f%%"), Efficiency * 100.0);
}

void UEnergyEfficiencyVisualizerComponent::UpdateDisplay(double Efficiency) const
{
	if (DisplayText)
	{
		const FString Text = FString::Printf(TEXT("Efficiency: %.2f%%"), Efficiency * 100.0);
		DisplayText->SetText(FText::FromString(Text));
	}
}
