#include "HistoricalPatternPredictionActor.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

//////////////////////////////////////////////////////////////////////////
// FHistoricalPatternNeuralNetwork

void FHistoricalPatternNeuralNetwork::Initialize(int32 InInputSize, int32 InHiddenSize)
{
	InputSize = InInputSize;
	HiddenSize = InHiddenSize;
	WeightsInputHidden.SetNum(InputSize * HiddenSize);
	BiasHidden.SetNum(HiddenSize);
	WeightsHiddenOutput.SetNum(HiddenSize);

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

TArray<double> FHistoricalPatternNeuralNetwork::Compute(const TArray<double>& Input)
{
	TArray<double> OutputArray;

	if (Input.Num() != InputSize || HiddenSize == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Neural network received mismatched input sizes."));
		return OutputArray;
	}

	TArray<double> HiddenActivations;
	HiddenActivations.SetNum(HiddenSize);

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
	OutputArray.Add(OutputValue);

	return OutputArray;
}

//////////////////////////////////////////////////////////////////////////
// AHistoricalPatternPredictionActor

AHistoricalPatternPredictionActor::AHistoricalPatternPredictionActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHistoricalPatternPredictionActor::BeginPlay()
{
	Super::BeginPlay();

	InitializeParameters();

	BaselineInfluence = 50.0;
	Amplitudes = { 10.0, 5.0, 2.0 };
	Periods = { 24.0, 12.0, 6.0 };
	PhaseShifts = { 0.0, PI / 4.0, PI / 2.0 };

	InitializeNeuralNetwork();

	const double Time = 12.0;

	CelestialInfluence = CalculateCelestialInfluence(BaselineInfluence, Amplitudes, Periods, PhaseShifts, Time);
	UE_LOG(LogTemp, Log, TEXT("Celestial Influence C(t) at t=%.2f: %.4f"), Time, CelestialInfluence);

	TArray<double> InputArray;
	InputArray.Add(BaselineInfluence);
	InputArray.Add(Time);
	InputArray = AppendListToArray(InputArray, Amplitudes);
	InputArray = AppendListToArray(InputArray, Periods);
	InputArray = AppendListToArray(InputArray, PhaseShifts);

	TArray<double> Output = PredictCelestialInfluence(InputArray);
	if (Output.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Celestial Influence C(t): %.4f"), Output[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Neural network did not produce a prediction."));
	}

	const double TensorFlowResult = ComputeWithTensorFlow(BaselineInfluence, Amplitudes, Periods, PhaseShifts, Time);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result C(t): %.4f"), TensorFlowResult);
}

void AHistoricalPatternPredictionActor::InitializeParameters()
{
	CelestialInfluence = 0.0;
	BaselineInfluence = 0.0;
	Amplitudes.Reset();
	Periods.Reset();
	PhaseShifts.Reset();
}

void AHistoricalPatternPredictionActor::InitializeNeuralNetwork()
{
	const int32 InputSize = 2 + Amplitudes.Num() * 3;
	NeuralNetwork.Initialize(InputSize, 10);
}

double AHistoricalPatternPredictionActor::CalculateCelestialInfluence(double C0, const TArray<double>& A, const TArray<double>& T, const TArray<double>& Phi, double t) const
{
	if (A.Num() != T.Num() || A.Num() != Phi.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Amplitudes, periods, and phase shifts arrays must have the same size."));
		return 0.0;
	}

	double TotalInfluence = C0;

	for (int32 Index = 0; Index < A.Num(); ++Index)
	{
		TotalInfluence += A[Index] * FMath::Cos((2.0 * PI * t / T[Index]) + Phi[Index]);
	}

	return TotalInfluence;
}

TArray<double> AHistoricalPatternPredictionActor::PredictCelestialInfluence(const TArray<double>& Input)
{
	return NeuralNetwork.Compute(Input);
}

double AHistoricalPatternPredictionActor::ComputeWithTensorFlow(double C0, const TArray<double>& A, const TArray<double>& T, const TArray<double>& Phi, double t) const
{
	if (A.Num() != T.Num() || A.Num() != Phi.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("TensorFlow computation requires matched harmonic counts."));
		return 0.0;
	}

	double TotalInfluence = C0;

	for (int32 Index = 0; Index < A.Num(); ++Index)
	{
		const double Term = A[Index] * FMath::Cos((2.0 * PI * t / T[Index]) + Phi[Index]);
		TotalInfluence += Term;
	}

	return TotalInfluence;
}

TArray<double> AHistoricalPatternPredictionActor::AppendListToArray(const TArray<double>& Array, const TArray<double>& List) const
{
	TArray<double> Result = Array;
	Result.Append(List);
	return Result;
}
