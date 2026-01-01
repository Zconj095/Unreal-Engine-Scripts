#include "MoonlightEnergyVisualizerActor.h"
#include "Math/UnrealMathUtility.h"

AMoonlightEnergyVisualizerActor::AMoonlightEnergyVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	DisplayText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MoonlightText"));
	SetRootComponent(DisplayText);
	DisplayText->SetHorizontalAlignment(EHTA_Center);
}

void AMoonlightEnergyVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (Distance == 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Distance cannot be zero."));
		return;
	}

	const float Flux = ComputeEnergyFlux();
	const FString Text = FString::Printf(TEXT("Moonlight Energy Flux: %.4f units"), Flux);
	DisplayText->SetText(FText::FromString(Text));
	UE_LOG(LogTemp, Log, TEXT("%s"), *Text);
}

float AMoonlightEnergyVisualizerActor::ComputeEnergyFlux() const
{
	return (Intensity * Area) / FMath::Pow(Distance, 2);
}
