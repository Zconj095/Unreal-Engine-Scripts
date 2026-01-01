#include "LeylineNeuralApproximatorComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

void FSimpleNeuralApproximator::Initialize()
{
	Weights.SetNumZeroed(InputSize * HiddenSize);
	Biases.SetNumZeroed(HiddenSize + 1);

	for (int32 Index = 0; Index < Weights.Num(); ++Index)
	{
		Weights[Index] = FMath::FRandRange(-1.0, 1.0);
	}

	for (int32 Index = 0; Index < Biases.Num(); ++Index)
	{
		Biases[Index] = FMath::FRandRange(-1.0, 1.0);
	}
}

double FSimpleNeuralApproximator::Predict(const TArray<double>& Input) const
{
	if (Input.Num() != InputSize)
	{
		return 0.0;
	}

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

	double Output = Biases.Last();
	for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
	{
		Output += Hidden[HiddenIndex];
	}

	return 1.0 / (1.0 + FMath::Exp(-Output));
}

ULeylineNeuralApproximatorComponent::ULeylineNeuralApproximatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineNeuralApproximatorComponent::BeginPlay()
{
	Super::BeginPlay();
	Network.Initialize();
	TrainMock();
}

void ULeylineNeuralApproximatorComponent::TrainMock()
{
	UE_LOG(LogTemp, Log, TEXT("Mock training completed (not actual learning)."));
}

double ULeylineNeuralApproximatorComponent::PredictEnergyDensity(const FVector& Position) const
{
	TArray<double> Input;
	Input.Add(Position.X);
	Input.Add(Position.Y);
	Input.Add(Position.Z);
	return Network.Predict(Input);
}
