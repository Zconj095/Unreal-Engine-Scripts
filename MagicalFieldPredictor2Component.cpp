#include "MagicalFieldPredictor2Component.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

void FFieldPredictor2Network::Initialize()
{
	Weights.SetNumZeroed(InputSize * HiddenSize + HiddenSize * OutputSize);
	Biases.SetNumZeroed(HiddenSize + OutputSize);

	for (double& Weight : Weights)
	{
		Weight = FMath::FRandRange(-1.0, 1.0);
	}
	for (double& Bias : Biases)
	{
		Bias = FMath::FRandRange(-1.0, 1.0);
	}
}

TArray<double> FFieldPredictor2Network::Predict(const TArray<double>& Input) const
{
	TArray<double> Hidden;
	Hidden.SetNumZeroed(HiddenSize);

	for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
	{
		double Sum = Biases[HiddenIndex];
		for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
		{
			Sum += Input[InputIndex] * Weights[HiddenIndex * InputSize + InputIndex];
		}
		Hidden[HiddenIndex] = 1.0 / (1.0 + FMath::Exp(-Sum));
	}

	TArray<double> Output;
	Output.SetNumZeroed(OutputSize);
	for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
	{
		double Sum = Biases[HiddenSize + OutputIndex];
		for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
		{
			Sum += Hidden[HiddenIndex] * Weights[InputSize * HiddenSize + OutputIndex * HiddenSize + HiddenIndex];
		}
		Output[OutputIndex] = 1.0 / (1.0 + FMath::Exp(-Sum));
	}

	return Output;
}

UMagicalFieldPredictor2Component::UMagicalFieldPredictor2Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMagicalFieldPredictor2Component::BeginPlay()
{
	Super::BeginPlay();
	Network.Initialize();
	UE_LOG(LogTemp, Log, TEXT("Magical field predictor 2 initialized (mock)."));
}
