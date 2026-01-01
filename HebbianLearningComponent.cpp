#include "HebbianLearningComponent.h"

#include "Logging/LogMacros.h"

UHebbianLearningComponent::UHebbianLearningComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHebbianLearningComponent::BeginPlay()
{
	Super::BeginPlay();

	NumNeurons = FMath::Max(1, NumNeurons);

	NeuronActivations.Init(0, NumNeurons);
	Weights.Init(0.f, NumNeurons * NumNeurons);

	InitializeWeights();
}

void UHebbianLearningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GenerateRandomActivations();
	ApplyHebbianLearning();

	if (bLogEachTick)
	{
		PrintNeuronsAndWeights();
	}
}

void UHebbianLearningComponent::InitializeWeights()
{
	for (int32 I = 0; I < NumNeurons; ++I)
	{
		for (int32 J = 0; J < NumNeurons; ++J)
		{
			if (I != J)
			{
				Weights[Idx(I, J)] = FMath::FRandRange(-0.1f, 0.1f);
			}
			else
			{
				Weights[Idx(I, J)] = 0.f;
			}
		}
	}
}

void UHebbianLearningComponent::GenerateRandomActivations()
{
	for (int32 I = 0; I < NumNeurons; ++I)
	{
		NeuronActivations[I] = FMath::RandRange(0, 1); // Binary 0 or 1
	}
}

void UHebbianLearningComponent::ApplyHebbianLearning()
{
	for (int32 I = 0; I < NumNeurons; ++I)
	{
		for (int32 J = 0; J < NumNeurons; ++J)
		{
			if (I != J)
			{
				Weights[Idx(I, J)] += LearningRate * static_cast<float>(NeuronActivations[I] * NeuronActivations[J]);
			}
		}
	}
}

void UHebbianLearningComponent::PrintNeuronsAndWeights() const
{
	FString ActivationsStr = TEXT("Neuron Activations: ");
	for (int32 I : NeuronActivations)
	{
		ActivationsStr += FString::FromInt(I) + TEXT(" ");
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *ActivationsStr);

	FString WeightsStr = TEXT("Weights Matrix:\n");
	for (int32 I = 0; I < NumNeurons; ++I)
	{
		for (int32 J = 0; J < NumNeurons; ++J)
		{
			WeightsStr += FString::Printf(TEXT("%.2f "), Weights[Idx(I, J)]);
		}
		WeightsStr += TEXT("\n");
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *WeightsStr);
}
