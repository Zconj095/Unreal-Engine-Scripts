#include "DynamicEnergyFlowPredictor.h"
#include "Math/UnrealMathUtility.h"
#include <initializer_list>

void UDynamicEnergyFlowPredictor::Initialize(int32 InputSize, int32 OutputSize)
{
	NumInputs = InputSize;
	NumOutputs = OutputSize;
	NumHidden = 10;
	InitializeWeights();
}

void UDynamicEnergyFlowPredictor::InitializeWeights()
{
	if (NumInputs <= 0 || NumOutputs <= 0)
	{
		return;
	}

	InputHiddenWeights.SetNum(NumHidden);
	for (int32 HiddenIndex = 0; HiddenIndex < NumHidden; ++HiddenIndex)
	{
		InputHiddenWeights[HiddenIndex].Init(0.0, NumInputs + 1);

		for (int32 InputIndex = 0; InputIndex < NumInputs + 1; ++InputIndex)
		{
			InputHiddenWeights[HiddenIndex][InputIndex] = FMath::FRandRange(-1.0, 1.0);
		}
	}

	HiddenOutputWeights.SetNum(NumOutputs);
	for (int32 OutputIndex = 0; OutputIndex < NumOutputs; ++OutputIndex)
	{
		HiddenOutputWeights[OutputIndex].Init(0.0, NumHidden + 1);

		for (int32 HiddenIndex = 0; HiddenIndex < NumHidden + 1; ++HiddenIndex)
		{
			HiddenOutputWeights[OutputIndex][HiddenIndex] = FMath::FRandRange(-1.0, 1.0);
		}
	}
}

TArray<double> UDynamicEnergyFlowPredictor::Predict(const TArray<double>& Inputs) const
{
	if (Inputs.Num() != NumInputs)
	{
		UE_LOG(LogTemp, Warning, TEXT("Predict input size %d does not match initialized size %d"), Inputs.Num(), NumInputs);
		return {};
	}

	TArray<double> HiddenOutputs;
	TArray<double> OutputValues;
	Forward(Inputs, HiddenOutputs, OutputValues);
	return OutputValues;
}

void UDynamicEnergyFlowPredictor::Forward(const TArray<double>& Inputs, TArray<double>& HiddenOutputs, TArray<double>& OutputValues) const
{
	HiddenOutputs.Reset();
	HiddenOutputs.AddZeroed(NumHidden);

	for (int32 HiddenIndex = 0; HiddenIndex < NumHidden; ++HiddenIndex)
	{
		double Total = InputHiddenWeights[HiddenIndex][NumInputs];

		for (int32 InputIndex = 0; InputIndex < NumInputs; ++InputIndex)
		{
			Total += InputHiddenWeights[HiddenIndex][InputIndex] * Inputs[InputIndex];
		}

		HiddenOutputs[HiddenIndex] = Activation(Total);
	}

	OutputValues.Reset();
	OutputValues.AddZeroed(NumOutputs);

	for (int32 OutputIndex = 0; OutputIndex < NumOutputs; ++OutputIndex)
	{
		double Total = HiddenOutputWeights[OutputIndex][NumHidden];

		for (int32 HiddenIndex = 0; HiddenIndex < NumHidden; ++HiddenIndex)
		{
			Total += HiddenOutputWeights[OutputIndex][HiddenIndex] * HiddenOutputs[HiddenIndex];
		}

		OutputValues[OutputIndex] = Activation(Total);
	}
}

void UDynamicEnergyFlowPredictor::Train(const TArray<FDoubleArrayWrapper>& Inputs, const TArray<FDoubleArrayWrapper>& Outputs, int32 Epochs, double LearningRate)
{
	if (NumInputs <= 0 || NumOutputs <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Initialize must be called before training."));
		return;
	}

	if (Inputs.Num() == 0 || Inputs.Num() != Outputs.Num() || Epochs <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Train requires matching non-empty datasets and positive epochs."));
		return;
	}

	TArray<double> HiddenOutputs;
	TArray<double> OutputValues;
	TArray<double> OutputErrors;
	TArray<double> HiddenErrors;

	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		double EpochError = 0.0;

		for (int32 SampleIndex = 0; SampleIndex < Inputs.Num(); ++SampleIndex)
		{
			const TArray<double>& SampleInput = Inputs[SampleIndex].Values;
			const TArray<double>& SampleTarget = Outputs[SampleIndex].Values;

			if (SampleInput.Num() != NumInputs || SampleTarget.Num() != NumOutputs)
			{
				continue;
			}

			Forward(SampleInput, HiddenOutputs, OutputValues);

			OutputErrors.SetNumZeroed(NumOutputs);
			HiddenErrors.SetNumZeroed(NumHidden);

			for (int32 OutputIndex = 0; OutputIndex < NumOutputs; ++OutputIndex)
			{
				const double ErrorDelta = SampleTarget[OutputIndex] - OutputValues[OutputIndex];
				OutputErrors[OutputIndex] = ErrorDelta * ActivationDerivative(OutputValues[OutputIndex]);
				EpochError += FMath::Square(ErrorDelta);
			}

			for (int32 HiddenIndex = 0; HiddenIndex < NumHidden; ++HiddenIndex)
			{
				double Sum = 0.0;

				for (int32 OutputIndex = 0; OutputIndex < NumOutputs; ++OutputIndex)
				{
					Sum += HiddenOutputWeights[OutputIndex][HiddenIndex] * OutputErrors[OutputIndex];
				}

				HiddenErrors[HiddenIndex] = Sum * ActivationDerivative(HiddenOutputs[HiddenIndex]);
			}

			for (int32 OutputIndex = 0; OutputIndex < NumOutputs; ++OutputIndex)
			{
				for (int32 HiddenIndex = 0; HiddenIndex < NumHidden; ++HiddenIndex)
				{
					HiddenOutputWeights[OutputIndex][HiddenIndex] += LearningRate * OutputErrors[OutputIndex] * HiddenOutputs[HiddenIndex];
				}

				HiddenOutputWeights[OutputIndex][NumHidden] += LearningRate * OutputErrors[OutputIndex];
			}

			for (int32 HiddenIndex = 0; HiddenIndex < NumHidden; ++HiddenIndex)
			{
				for (int32 InputIndex = 0; InputIndex < NumInputs; ++InputIndex)
				{
					InputHiddenWeights[HiddenIndex][InputIndex] += LearningRate * HiddenErrors[HiddenIndex] * SampleInput[InputIndex];
				}

				InputHiddenWeights[HiddenIndex][NumInputs] += LearningRate * HiddenErrors[HiddenIndex];
			}
		}

		if (Epoch % 100 == 0)
		{
			const double AverageError = EpochError / Inputs.Num();
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d, Error: %.6f"), Epoch + 1, Epochs, AverageError);
		}
	}
}

void UDynamicEnergyFlowPredictor::Test()
{
	UDynamicEnergyFlowPredictor* Predictor = NewObject<UDynamicEnergyFlowPredictor>();
	Predictor->Initialize(3, 1);

	auto MakeRow = [](const std::initializer_list<double>& Values)
	{
		FDoubleArrayWrapper Wrapper;
		Wrapper.Values.Append(Values);
		return Wrapper;
	};

	TArray<FDoubleArrayWrapper> Inputs;
	Inputs.Add(MakeRow({10.0, 0.2, 2.0 * PI / 24.0}));
	Inputs.Add(MakeRow({5.0, 0.1, 2.0 * PI / 12.0}));

	TArray<FDoubleArrayWrapper> Outputs;
	Outputs.Add(MakeRow({8.5}));
	Outputs.Add(MakeRow({4.2}));

	Predictor->Train(Inputs, Outputs, 1000, 0.01);

	TArray<double> PredictionInput;
	PredictionInput.Add(8.0);
	PredictionInput.Add(0.15);
	PredictionInput.Add(2.0 * PI / 18.0);
	const TArray<double> Predicted = Predictor->Predict(PredictionInput);

	if (Predicted.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Energy: %.4f"), Predicted[0]);
	}
}

double UDynamicEnergyFlowPredictor::Activation(double Value) const
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}

double UDynamicEnergyFlowPredictor::ActivationDerivative(double Value) const
{
	return Value * (1.0 - Value);
}
