#include "EcosystemEnergyFlowPredictor.h"
#include "Math/UnrealMathUtility.h"
#include <initializer_list>

void UEcosystemEnergyFlowPredictor::Initialize(int32 InInputDim, int32 InOutputDim)
{
	InputDim = FMath::Max(1, InInputDim);
	OutputDim = FMath::Max(1, InOutputDim);
	HiddenDim = 10;
	InitializeWeights();
}

void UEcosystemEnergyFlowPredictor::InitializeWeights()
{
	InputHiddenWeights.SetNum(HiddenDim);
	for (int32 HiddenIndex = 0; HiddenIndex < HiddenDim; ++HiddenIndex)
	{
		InputHiddenWeights[HiddenIndex].Init(0.0, InputDim + 1);
		for (int32 InputIndex = 0; InputIndex < InputDim + 1; ++InputIndex)
		{
			InputHiddenWeights[HiddenIndex][InputIndex] = FMath::FRandRange(-1.0, 1.0);
		}
	}

	HiddenOutputWeights.SetNum(OutputDim);
	for (int32 OutputIndex = 0; OutputIndex < OutputDim; ++OutputIndex)
	{
		HiddenOutputWeights[OutputIndex].Init(0.0, HiddenDim + 1);
		for (int32 HiddenIndex = 0; HiddenIndex < HiddenDim + 1; ++HiddenIndex)
		{
			HiddenOutputWeights[OutputIndex][HiddenIndex] = FMath::FRandRange(-1.0, 1.0);
		}
	}
}

TArray<double> UEcosystemEnergyFlowPredictor::Predict(const TArray<double>& Inputs) const
{
	if (!ensure(Inputs.Num() == InputDim))
	{
		UE_LOG(LogTemp, Warning, TEXT("Predict input dimension mismatch."));
		return {};
	}

	TArray<double> Hidden;
	TArray<double> Outputs;
	Forward(Inputs, Hidden, Outputs);
	return Outputs;
}

void UEcosystemEnergyFlowPredictor::Forward(const TArray<double>& Inputs, TArray<double>& HiddenOutputs, TArray<double>& OutputValues) const
{
	HiddenOutputs.Reset();
	HiddenOutputs.AddZeroed(HiddenDim);

	for (int32 HiddenIndex = 0; HiddenIndex < HiddenDim; ++HiddenIndex)
	{
		double Total = InputHiddenWeights[HiddenIndex][InputDim];
		for (int32 InputIndex = 0; InputIndex < InputDim; ++InputIndex)
		{
			Total += InputHiddenWeights[HiddenIndex][InputIndex] * Inputs[InputIndex];
		}
		HiddenOutputs[HiddenIndex] = Activation(Total);
	}

	OutputValues.Reset();
	OutputValues.AddZeroed(OutputDim);

	for (int32 OutputIndex = 0; OutputIndex < OutputDim; ++OutputIndex)
	{
		double Total = HiddenOutputWeights[OutputIndex][HiddenDim];
		for (int32 HiddenIndex = 0; HiddenIndex < HiddenDim; ++HiddenIndex)
		{
			Total += HiddenOutputWeights[OutputIndex][HiddenIndex] * HiddenOutputs[HiddenIndex];
		}
		OutputValues[OutputIndex] = Activation(Total);
	}
}

void UEcosystemEnergyFlowPredictor::Train(const TArray<FDoubleArrayWrapper>& Inputs, const TArray<FDoubleArrayWrapper>& Outputs, int32 Epochs, double LearningRate)
{
	if (Inputs.Num() == 0 || Outputs.Num() != Inputs.Num() || Epochs <= 0 || LearningRate <= 0.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid training configuration."));
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

			if (SampleInput.Num() != InputDim || SampleTarget.Num() != OutputDim)
			{
				continue;
			}

			Forward(SampleInput, HiddenOutputs, OutputValues);

			OutputErrors.SetNumZeroed(OutputDim);
			HiddenErrors.SetNumZeroed(HiddenDim);

			for (int32 OutputIndex = 0; OutputIndex < OutputDim; ++OutputIndex)
			{
				const double Error = SampleTarget[OutputIndex] - OutputValues[OutputIndex];
				OutputErrors[OutputIndex] = Error * ActivationDerivative(OutputValues[OutputIndex]);
				EpochError += FMath::Square(Error);
			}

			for (int32 HiddenIndex = 0; HiddenIndex < HiddenDim; ++HiddenIndex)
			{
				double Sum = 0.0;
				for (int32 OutputIndex = 0; OutputIndex < OutputDim; ++OutputIndex)
				{
					Sum += HiddenOutputWeights[OutputIndex][HiddenIndex] * OutputErrors[OutputIndex];
				}
				HiddenErrors[HiddenIndex] = Sum * ActivationDerivative(HiddenOutputs[HiddenIndex]);
			}

			for (int32 OutputIndex = 0; OutputIndex < OutputDim; ++OutputIndex)
			{
				for (int32 HiddenIndex = 0; HiddenIndex < HiddenDim; ++HiddenIndex)
				{
					HiddenOutputWeights[OutputIndex][HiddenIndex] += LearningRate * OutputErrors[OutputIndex] * HiddenOutputs[HiddenIndex];
				}
				HiddenOutputWeights[OutputIndex][HiddenDim] += LearningRate * OutputErrors[OutputIndex];
			}

			for (int32 HiddenIndex = 0; HiddenIndex < HiddenDim; ++HiddenIndex)
			{
				for (int32 InputIndex = 0; InputIndex < InputDim; ++InputIndex)
				{
					InputHiddenWeights[HiddenIndex][InputIndex] += LearningRate * HiddenErrors[HiddenIndex] * SampleInput[InputIndex];
				}
				InputHiddenWeights[HiddenIndex][InputDim] += LearningRate * HiddenErrors[HiddenIndex];
			}
		}

		if (Epoch % 100 == 0)
		{
			const double AverageError = EpochError / Inputs.Num();
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d, Error: %.6f"), Epoch + 1, Epochs, AverageError);
		}
	}
}

void UEcosystemEnergyFlowPredictor::Test()
{
    UEcosystemEnergyFlowPredictor* Predictor = NewObject<UEcosystemEnergyFlowPredictor>();
    Predictor->Initialize(2, 1);

    auto BuildRow = [](const std::initializer_list<double>& Values)
    {
        FDoubleArrayWrapper Wrapper;
        Wrapper.Values.Append(Values);
        return Wrapper;
    };

    TArray<FDoubleArrayWrapper> Inputs;
    Inputs.Add(BuildRow({100.0, 0.8}));
    Inputs.Add(BuildRow({80.0, 0.5}));

    TArray<FDoubleArrayWrapper> Outputs;
    Outputs.Add(BuildRow({80.0}));
    Outputs.Add(BuildRow({40.0}));

    Predictor->Train(Inputs, Outputs, 1000, 0.01);

    const TArray<double> PredictionInput = {90.0, 0.7};
    const TArray<double> Prediction = Predictor->Predict(PredictionInput);

    if (Prediction.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Predicted Total Energy Flow: %.4f"), Prediction[0]);
    }
}

double UEcosystemEnergyFlowPredictor::Activation(double Value) const
{
	return 1.0 / (1.0 + FMath::Exp(-Value));
}

double UEcosystemEnergyFlowPredictor::ActivationDerivative(double Value) const
{
	return Value * (1.0 - Value);
}
