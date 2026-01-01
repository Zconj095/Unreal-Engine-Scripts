#include "MagicalNeuralNetworkActor.h"
#include "Math/UnrealMathUtility.h"

AMagicalNeuralNetworkActor::AMagicalNeuralNetworkActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMagicalNeuralNetworkActor::BeginPlay()
{
	Super::BeginPlay();

	SimulateNetwork();
}

void AMagicalNeuralNetworkActor::SimulateNetwork()
{
	const TArray<float> Inputs = { 0.f, 1.f, 1.f };
	const int32 LayerCount = HiddenLayerSizes.Num();

	TArray<int32> Architecture;
	Architecture.Add(InputSize);
	Architecture.Append(HiddenLayerSizes);
	Architecture.Add(OutputSize);

	UE_LOG(LogTemp, Log, TEXT("Simulating Magical Neural Network with %d layers."), Architecture.Num());
	for (int32 LayerIndex = 0; LayerIndex < Architecture.Num() - 1; ++LayerIndex)
	{
		const int32 CurrentLayerSize = Architecture[LayerIndex];
		const int32 NextLayerSize = Architecture[LayerIndex + 1];

		UE_LOG(LogTemp, Log, TEXT("Layer %d -> %d neurons."), CurrentLayerSize, NextLayerSize);
	}

	float Activation = 0.f;
	for (float Value : Inputs)
	{
		Activation += Value;
	}

	Activation = FMath::Clamp(Activation / Inputs.Num(), 0.f, 1.f);
	UE_LOG(LogTemp, Log, TEXT("Forward pass result: %f"), Activation);
}
