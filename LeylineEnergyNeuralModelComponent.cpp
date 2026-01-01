#include "LeylineEnergyNeuralModelComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

void FLeylineEnergyNeuralNetwork::Initialize()
{
	WeightsInputHidden.SetNumZeroed(InputSize * HiddenSize);
	BiasHidden.SetNumZeroed(HiddenSize);
	WeightsHiddenOutput.SetNumZeroed(HiddenSize);

	for (int32 Index = 0; Index < InputSize * HiddenSize; ++Index)
	{
		WeightsInputHidden[Index] = FMath::FRandRange(-1.0, 1.0);
	}

	for (int32 Index = 0; Index < HiddenSize; ++Index)
	{
		BiasHidden[Index] = FMath::FRandRange(-1.0, 1.0);
		WeightsHiddenOutput[Index] = FMath::FRandRange(-1.0, 1.0);
	}

	BiasOutput = FMath::FRandRange(-1.0, 1.0);
}

double FLeylineEnergyNeuralNetwork::Predict(double Phi, double Gradient) const
{
	TArray<double> HiddenActivations;
	HiddenActivations.SetNumZeroed(HiddenSize);

	for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
	{
		double Sum = BiasHidden[HiddenIndex];

		for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
		{
			const double InputValue = (InputIndex == 0) ? Phi : Gradient;
			Sum += InputValue * WeightsInputHidden[HiddenIndex * InputSize + InputIndex];
		}
		HiddenActivations[HiddenIndex] = 1.0 / (1.0 + FMath::Exp(-Sum));
	}

	double OutputSum = BiasOutput;
	for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
	{
		OutputSum += HiddenActivations[HiddenIndex] * WeightsHiddenOutput[HiddenIndex];
	}

	return 1.0 / (1.0 + FMath::Exp(-OutputSum));
}

ULeylineEnergyNeuralModelComponent::ULeylineEnergyNeuralModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyNeuralModelComponent::BeginPlay()
{
	Super::BeginPlay();
	Network.Initialize();
}

void ULeylineEnergyNeuralModelComponent::TrainAndLog()
{
	UE_LOG(LogTemp, Log, TEXT("Training placeholder - no actual learning performed in this simplified component."));
	UE_LOG(LogTemp, Log, TEXT("Predicted Energy Flow: %.4f"), PredictEnergyFlow(0.8, 0.6));
}

double ULeylineEnergyNeuralModelComponent::PredictEnergyFlow(double Phi, double Gradient) const
{
	return Network.Predict(Phi, Gradient);
}
