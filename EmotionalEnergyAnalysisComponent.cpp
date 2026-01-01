#include "EmotionalEnergyAnalysisComponent.h"
#include "Math/UnrealMathUtility.h"

UEmotionalEnergyAnalysisComponent::UEmotionalEnergyAnalysisComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InitializeParameters();
}

void UEmotionalEnergyAnalysisComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeNeuralNetwork();
	InitializeTensorFlow();

	// Example values to seed the component for debugging.
	Alpha = {1.0, 0.8, 0.6};
	Distances = {1.0, 2.0, 3.0};
	Beta = 0.5;

	TotalEmotionalEnergy = CalculateEmotionalEnergy(Alpha, Beta, Distances);
	UE_LOG(LogTemp, Log, TEXT("Total Emotional Energy (E_emotion): %f"), TotalEmotionalEnergy);

	TArray<double> Input = {Alpha[0], Distances[0], Beta};
	TArray<double> Prediction = PredictEmotionalEnergy(Input);
	if (Prediction.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Emotional Energy: %f"), Prediction[0]);
	}

	double TfResult = ComputeWithTensorFlow(Alpha, Beta, Distances);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (E_emotion): %f"), TfResult);
}

void UEmotionalEnergyAnalysisComponent::InitializeParameters()
{
	TotalEmotionalEnergy = 0.0;
	Alpha.Reset();
	Distances.Reset();
	Beta = 0.0;
	HiddenLayerWeights.Reset();
	OutputWeight = 1.0;
}

void UEmotionalEnergyAnalysisComponent::InitializeNeuralNetwork()
{
	// Placeholder weights for a single hidden-unit perceptron.
	HiddenLayerWeights = {0.25, 0.15, 0.35};
	OutputWeight = 0.6;
}

void UEmotionalEnergyAnalysisComponent::InitializeTensorFlow()
{
	// In a full integration this is where a TensorFlow session or runtime would be prepared.
}

double UEmotionalEnergyAnalysisComponent::CalculateEmotionalEnergy(const TArray<double>& InAlpha, double InBeta, const TArray<double>& InDistances) const
{
	double TotalEnergy = 0.0;
	const int32 Count = FMath::Min(InAlpha.Num(), InDistances.Num());

	for (int32 Index = 0; Index < Count; ++Index)
	{
		TotalEnergy += InAlpha[Index] * FMath::Exp(-InBeta * InDistances[Index]);
	}

	return TotalEnergy;
}

TArray<double> UEmotionalEnergyAnalysisComponent::PredictEmotionalEnergy(const TArray<double>& Input) const
{
	if (HiddenLayerWeights.Num() == 0 || Input.Num() == 0)
	{
		return {};
	}

	double Sum = 0.0;
	const int32 Count = FMath::Min(HiddenLayerWeights.Num(), Input.Num());
	for (int32 Index = 0; Index < Count; ++Index)
	{
		Sum += Input[Index] * HiddenLayerWeights[Index];
	}

	auto Sigmoid = [](double Value)
	{
		return 1.0 / (1.0 + FMath::Exp(-Value));
	};

	double HiddenOutput = Sigmoid(Sum);
	double Output = Sigmoid(HiddenOutput * OutputWeight);
	return {Output};
}

double UEmotionalEnergyAnalysisComponent::ComputeWithTensorFlow(const TArray<double>& InAlpha, double InBeta, const TArray<double>& InDistances) const
{
	// This mimics the TensorFlow computation locally; replace with actual TF runtime calls as needed.
	return CalculateEmotionalEnergy(InAlpha, InBeta, InDistances);
}
