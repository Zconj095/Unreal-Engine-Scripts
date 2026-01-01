#include "LunarPhaseVisualizerActor.h"
#include "Engine/Engine.h"

ALunarPhaseVisualizerActor::ALunarPhaseVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	DisplayText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DisplayText"));
	DisplayText->SetupAttachment(RootComponent);
	DisplayText->SetHorizontalAlignment(EHTA_Center);
	DisplayText->SetWorldSize(50.0f);

	PhaseComponent = CreateDefaultSubobject<ULunarPhasePredictionComponent>(TEXT("PhaseComponent"));
}

void ALunarPhaseVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (!PhaseComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Phase component missing."));
		return;
	}

	const double PhaseAngle = PhaseComponent->GetPhaseAngle(PI / 6.0, PI / 3.0);
	const FString Text = FString::Printf(TEXT("Lunar Phase Angle: %.4f radians"), PhaseAngle);
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(Text));
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *Text);
}
