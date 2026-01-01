#include "EntropicEfficiencyComponent.h"
#include "Math/UnrealMathUtility.h"

UEntropicEfficiencyComponent::UEntropicEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	EnergyInformation = 1.0f;
	Entropy = 1.0f;
	Efficiency = 0.0f;
}

void UEntropicEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEntropicEfficiencyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Efficiency = CalculateEfficiency(EnergyInformation, Entropy);
	LogEfficiency();
}

float UEntropicEfficiencyComponent::CalculateEfficiency(float EnergyContent, float EntropyValue) const
{
	if (EntropyValue <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entropy (ΔS) must be greater than zero to calculate efficiency."));
		return 0.0f;
	}

	return EnergyContent / EntropyValue;
}

void UEntropicEfficiencyComponent::LogEfficiency() const
{
	UE_LOG(LogTemp, Log, TEXT("Energy Information Content (H): %.2f"), EnergyInformation);
	UE_LOG(LogTemp, Log, TEXT("Entropy (ΔS): %.2f"), Entropy);
	UE_LOG(LogTemp, Log, TEXT("Entropic Efficiency (η_s): %.2f"), Efficiency);
}
