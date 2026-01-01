#include "QuantumOperationQubitComponent.h"

UQuantumOperationQubitComponent::UQuantumOperationQubitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UQuantumOperationQubitComponent::Measure() const
{
	return 1.0f;
}
