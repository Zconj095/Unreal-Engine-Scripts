#include "MysticalLunarVisualizerActor.h"
#include "Components/TextRenderComponent.h"
#include "Math/UnrealMathUtility.h"

AMysticalLunarVisualizerActor::AMysticalLunarVisualizerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    DisplayText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DisplayText"));
    DisplayText->SetupAttachment(RootComponent);
    DisplayText->SetHorizontalAlignment(EHTA_Center);
    DisplayText->SetVerticalAlignment(EVRTA_TextCenter);
    DisplayText->SetTextRenderColor(FColor::Cyan);
}

void AMysticalLunarVisualizerActor::BeginPlay()
{
    Super::BeginPlay();

    if (SampleCount < 2)
    {
        SampleCount = 2;
        UE_LOG(LogTemp, Warning, TEXT("Sample count must be at least 2. Clamping to 2."));
    }

    if (TimeEnd <= TimeStart)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeEnd must be greater than TimeStart; skipping visualization."));
        return;
    }

    double LunarEnergyFlow = IntegrateLunarEnergy();
    FString Label = FString::Printf(TEXT("Lunar Energy Transfer: %.4f units"), LunarEnergyFlow);
    DisplayText->SetText(FText::FromString(Label));
    UE_LOG(LogTemp, Log, TEXT("%s"), *Label);
}

double AMysticalLunarVisualizerActor::Phi(double Time) const
{
    return FMath::Sin(Time);
}

double AMysticalLunarVisualizerActor::GradientPsi(double Time) const
{
    return FMath::Cos(Time);
}

double AMysticalLunarVisualizerActor::IntegrateLunarEnergy() const
{
    double Interval = TimeEnd - TimeStart;
    double StepSize = Interval / SampleCount;

    auto Integrand = [this](double Time) { return Phi(Time) * GradientPsi(Time); };

    double Sum = 0.0;
    for (int32 Index = 1; Index < SampleCount; ++Index)
    {
        double SampleTime = TimeStart + Index * StepSize;
        Sum += Integrand(SampleTime);
    }

    double StartValue = Integrand(TimeStart);
    double EndValue = Integrand(TimeEnd);

    return StepSize * (0.5 * (StartValue + EndValue) + Sum);
}
