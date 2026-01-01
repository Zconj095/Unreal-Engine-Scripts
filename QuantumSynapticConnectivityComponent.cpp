#include "QuantumSynapticConnectivityComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UQuantumSynapticConnectivityComponent::UQuantumSynapticConnectivityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuantumSynapticConnectivityComponent::BeginPlay()
{
	Super::BeginPlay();
	QuantumSynapticConnectivity = BaselineTunnelingProbability * FMath::Exp(-MaterialConstant * SynapticGapLength);
}

void UQuantumSynapticConnectivityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	QuantumSynapticConnectivity = BaselineTunnelingProbability * FMath::Exp(-MaterialConstant * SynapticGapLength);
	UE_LOG(LogTemp, Log, TEXT("Quantum Synaptic Connectivity: %0.4f"), QuantumSynapticConnectivity);
}
