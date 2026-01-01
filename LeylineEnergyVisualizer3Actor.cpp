#include "LeylineEnergyVisualizer3Actor.h"
#include "Engine/Engine.h"

ALeylineEnergyVisualizer3Actor::ALeylineEnergyVisualizer3Actor()
{
	PrimaryActorTick.bCanEverTick = false;
	DisplayText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DisplayText"));
	DisplayText->SetupAttachment(RootComponent);
	DisplayText->SetHorizontalAlignment(EHTA_Center);
	DisplayText->SetWorldSize(50.0f);
}

void ALeylineEnergyVisualizer3Actor::BeginPlay()
{
	Super::BeginPlay();

	const double Efficiency = ULeylineEnergyInfusionUtility::CalculateEfficiency(OutputEnergy, InputEnergy);
	const FString Text = FString::Printf(TEXT("Efficiency: %.2f"), Efficiency);
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(Text));
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Text);
}
