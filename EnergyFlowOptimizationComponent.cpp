#include "EnergyFlowOptimizationComponent.h"

UEnergyFlowOptimizationComponent::UEnergyFlowOptimizationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InitialEnergy = 10.0f;
	EnergyChangeRate = 5.0f;
	TimeInterval = 1.0f;
	CurrentEnergy = 0.0f;
	EnergyFlowRate = 0.0f;
	ElapsedTime = 0.0f;
	LastEnergy = 0.0f;
}

void UEnergyFlowOptimizationComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentEnergy = InitialEnergy;
	LastEnergy = InitialEnergy;
}

void UEnergyFlowOptimizationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;
	CurrentEnergy = InitialEnergy + EnergyChangeRate * ElapsedTime;

	if (ElapsedTime >= TimeInterval)
	{
		const float DeltaQ = CurrentEnergy - LastEnergy;
		EnergyFlowRate = DeltaQ / TimeInterval;
		LastEnergy = CurrentEnergy;
		ElapsedTime = 0.0f;
		LogStates();
	}
}

void UEnergyFlowOptimizationComponent::LogStates() const
{
	UE_LOG(LogTemp, Log, TEXT("Current Energy (Q): %.2f"), CurrentEnergy);
	UE_LOG(LogTemp, Log, TEXT("Energy Flow Rate (F_e): %.2f"), EnergyFlowRate);
}
