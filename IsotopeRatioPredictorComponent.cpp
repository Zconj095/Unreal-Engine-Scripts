#include "IsotopeRatioPredictorComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include <initializer_list>

//////////////////////////////////////////////////////////////////////////
// FIsotopeRatioNeuralNetwork

void FIsotopeRatioNeuralNetwork::Initialize(int32 InInputSize, int32 InHiddenSize, int32 InOutputSize)
{
	InputSize = InInputSize;
	HiddenSize = InHiddenSize;
	OutputSize = InOutputSize;

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

double FIsotopeRatioNeuralNetwork::FeedForward(const TArray<double>& Input, TArray<double>& HiddenActivations) const
{
	if (Input.Num() != InputSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Neural network received input with mismatched size."));
		return 0.0;
	}

	HiddenActivations.SetNumZeroed(HiddenSize);

	for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
	{
		double Sum = BiasHidden[HiddenIndex];

		for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
		{
			Sum += Input[InputIndex] * WeightsInputHidden[HiddenIndex * InputSize + InputIndex];
		}

		HiddenActivations[HiddenIndex] = 1.0 / (1.0 + FMath::Exp(-Sum));
	}

	double OutputSum = BiasOutput;

	for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
	{
		OutputSum += HiddenActivations[HiddenIndex] * WeightsHiddenOutput[HiddenIndex];
	}

	double OutputValue = 1.0 / (1.0 + FMath::Exp(-OutputSum));
	return OutputValue;
}

//////////////////////////////////////////////////////////////////////////
// UIsotopeRatioPredictorComponent

UIsotopeRatioPredictorComponent::UIsotopeRatioPredictorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UIsotopeRatioPredictorComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeNetwork();
}

void UIsotopeRatioPredictorComponent::InitializeNetwork()
{
	Network.Initialize(InputSize, HiddenSize, OutputSize);
}

TArray<double> UIsotopeRatioPredictorComponent::PredictRatios(const TArray<double>& Inputs)
{
	TArray<double> HiddenActivations;
	double OutputValue = Network.FeedForward(Inputs, HiddenActivations);
	TArray<double> Result;
	Result.Add(OutputValue);
	return Result;
}

void UIsotopeRatioPredictorComponent::Train(const TArray<FDoubleArrayWrapper>& Inputs, const TArray<FDoubleArrayWrapper>& Outputs, int32 Epochs, double LearningRate)
{
	if (Inputs.Num() != Outputs.Num() || Inputs.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Training requires matching input and output datasets."));
		return;
	}

	LearningRate = FMath::Max(LearningRate, KINDA_SMALL_NUMBER);

	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		double EpochError = 0.0;

		for (int32 SampleIndex = 0; SampleIndex < Inputs.Num(); ++SampleIndex)
		{
			const TArray<double>& InputSample = Inputs[SampleIndex].Values;
			const TArray<double>& OutputSample = Outputs[SampleIndex].Values;

			if (InputSample.Num() != InputSize || OutputSample.Num() != OutputSize)
			{
				continue;
			}

			TArray<double> HiddenActivations;
			double PredictedValue = Network.FeedForward(InputSample, HiddenActivations);
			double TargetValue = OutputSample[0];
			double OutputError = TargetValue - PredictedValue;
			EpochError += OutputError * OutputError;

			double OutputDelta = OutputError * PredictedValue * (1.0 - PredictedValue);

			for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
			{
				Network.WeightsHiddenOutput[HiddenIndex] += LearningRate * OutputDelta * HiddenActivations[HiddenIndex];
			}

			Network.BiasOutput += LearningRate * OutputDelta;

			for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
			{
				double HiddenError = Network.WeightsHiddenOutput[HiddenIndex] * OutputDelta;
				double HiddenDelta = HiddenActivations[HiddenIndex] * (1.0 - HiddenActivations[HiddenIndex]) * HiddenError;

				for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
				{
					Network.WeightsInputHidden[HiddenIndex * InputSize + InputIndex] += LearningRate * HiddenDelta * InputSample[InputIndex];
				}

				Network.BiasHidden[HiddenIndex] += LearningRate * HiddenDelta;
			}
		}

		if (Epoch % 100 == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d, Error: %.6f"), Epoch + 1, Epochs, EpochError);
		}
	}
}

void UIsotopeRatioPredictorComponent::RunExampleTraining()
{
	auto MakeRow = [](double Value)
	{
		FDoubleArrayWrapper Wrapper;
		Wrapper.Values.Add(Value);
		return Wrapper;
	};

	TArray<FDoubleArrayWrapper> Inputs;
	Inputs.Add(MakeRow(300.0));
	Inputs.Add(MakeRow(310.0));

	TArray<FDoubleArrayWrapper> Outputs;
	Outputs.Add(MakeRow(0.011200));
	Outputs.Add(MakeRow(0.011250));

	Train(Inputs, Outputs, 1000, 0.01);

	TArray<double> Query;
	Query.Add(305.0);
	TArray<double> Prediction = PredictRatios(Query);

	if (Prediction.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Isotope Ratio: %.6f"), Prediction[0]);
	}
}
