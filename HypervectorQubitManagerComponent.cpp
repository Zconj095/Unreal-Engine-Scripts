#include "HypervectorQubitManagerComponent.h"

UHypervectorQubitManagerComponent::UHypervectorQubitManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHypervectorQubitManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeHypervectors();
	InitializeQuantumOperation();
}

void UHypervectorQubitManagerComponent::InitializeHypervectors()
{
	Hypervectors.Reset();
	Hypervectors.SetNumZeroed(HypervectorCount);

	for (FFloatArrayWrapper& Vector : Hypervectors)
	{
		Vector.Values.SetNumZeroed(HypervectorDimension);
	}
}

void UHypervectorQubitManagerComponent::InitializeQuantumOperation()
{
	QuantumOperation.Name = TEXT("QuantumOperationQuBit");
}
