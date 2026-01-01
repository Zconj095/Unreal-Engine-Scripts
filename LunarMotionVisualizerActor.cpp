#include "LunarMotionVisualizerActor.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ALunarMotionVisualizerActor::ALunarMotionVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	DisplayText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DisplayText"));
	DisplayText->SetupAttachment(RootComponent);
	DisplayText->SetHorizontalAlignment(EHTA_Center);
	DisplayText->SetRelativeLocation(FVector(0, 0, 100));
	DisplayText->SetWorldSize(50.0f);
}

void ALunarMotionVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	const double Distance = CalculateDistance(SemiMajorAxis, Eccentricity, Anomaly);
	const FString Message = FString::Printf(TEXT("Distance to Moon: %.2f km"), Distance);
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(Message));
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}

double ALunarMotionVisualizerActor::CalculateDistance(double A, double E, double Theta) const
{
	if (FMath::IsNearlyZero(1.0 - FMath::Square(float(E))))
	{
		return A;
	}

	return A * (1.0 - FMath::Square(float(E))) / (1.0 + E * FMath::Cos(Theta));
}
