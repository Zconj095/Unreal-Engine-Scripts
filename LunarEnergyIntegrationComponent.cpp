#include "LunarEnergyIntegrationComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarEnergyIntegrationComponent::ULunarEnergyIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarEnergyIntegrationComponent::BeginPlay()
{
	Super::BeginPlay();
	IntegratedEnergy = CalculateLEI(InitialTime, InitialTime);
	LogEnergy(IntegratedEnergy);
}

void ULunarEnergyIntegrationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const float CurrentTime = InitialTime + DeltaTime;
	IntegratedEnergy = CalculateLEI(InitialTime, CurrentTime);
	LogEnergy(IntegratedEnergy);
}

float ULunarEnergyIntegrationComponent::CalculateLEI(float Start, float End) const
{
	float EnergySum = 0.0f;
	for (float Time = Start; Time < End; Time += TimeStep)
	{
		EnergySum += EnergyFlux(Time) * TimeStep;
	}

	return EnergySum;
}

float ULunarEnergyIntegrationComponent::EnergyFlux(float Time) const
{
	return FMath::Sin(2.0f * PI * Wavelength * Time);
}

void ULunarEnergyIntegrationComponent::LogEnergy(float Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Lunar Energy Integrated (El): %.2f"), Value);
}
