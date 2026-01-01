#include "QuantumNeuralNetworkComponent.h"
#include "Logging/LogMacros.h"

UQuantumNeuralNetworkComponent::UQuantumNeuralNetworkComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumNeuralNetworkComponent::BeginPlay()
{
	Super::BeginPlay();

	ModelDescription = FString::Printf(TEXT("Quantum Neural Network: input=%d, hidden=%d, output=%d"),
		InputSize, HiddenSize, HiddenSize);

	UE_LOG(LogTemp, Log, TEXT("%s"), *ModelDescription);
	UE_LOG(LogTemp, Log, TEXT("Simulated TensorFlow layer: output = input * weights + biases"));
}
