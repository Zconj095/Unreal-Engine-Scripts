#include "DynamicLossOptimizer.h"
#include "Math/UnrealMathUtility.h"

UDynamicLossOptimizer::UDynamicLossOptimizer()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDynamicLossOptimizer::BeginPlay()
{
	Super::BeginPlay();

	InitializeVariables();
}

void UDynamicLossOptimizer::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bInitialized)
	{
		return;
	}

	ForwardPass();
	BackwardPass();
}

void UDynamicLossOptimizer::InitializeVariables()
{
	if (InputDimension <= 0 || HiddenDimension <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions (Input=%d, Hidden=%d)."), InputDimension, HiddenDimension);
		bInitialized = false;
		return;
	}

	Input.SetNumZeroed(InputDimension);
	Output.SetNumZeroed(InputDimension);
	Hidden.SetNumZeroed(HiddenDimension);
	Mean.SetNumZeroed(InputDimension);
	StdDev.SetNumZeroed(InputDimension);

	WeightsInputHidden.SetNumZeroed(InputDimension * HiddenDimension);
	WeightsHiddenOutput.SetNumZeroed(HiddenDimension * InputDimension);

	InitializeWeights(WeightsInputHidden, InputDimension, HiddenDimension);
	InitializeWeights(WeightsHiddenOutput, HiddenDimension, InputDimension);

	bInitialized = true;
}

void UDynamicLossOptimizer::InitializeWeights(TArray<float>& Weights, int32 Rows, int32 Cols)
{
	for (int32 Row = 0; Row < Rows; ++Row)
	{
		for (int32 Col = 0; Col < Cols; ++Col)
		{
			const int32 Index = GetIndex(Row, Col, Cols);
			Weights[Index] = FMath::FRandRange(-0.1f, 0.1f);
		}
	}
}

void UDynamicLossOptimizer::ForwardPass()
{
	for (int32 HiddenIndex = 0; HiddenIndex < HiddenDimension; ++HiddenIndex)
	{
		float Accumulator = 0.0f;
		for (int32 InputIndex = 0; InputIndex < InputDimension; ++InputIndex)
		{
			Accumulator += Input[InputIndex] * WeightsInputHidden[GetIndex(InputIndex, HiddenIndex, HiddenDimension)];
		}
		Hidden[HiddenIndex] = Sigmoid(Accumulator);
	}

	for (int32 InputIndex = 0; InputIndex < InputDimension; ++InputIndex)
	{
		float Accumulator = 0.0f;
		for (int32 HiddenIndex = 0; HiddenIndex < HiddenDimension; ++HiddenIndex)
		{
			Accumulator += Hidden[HiddenIndex] * WeightsHiddenOutput[GetIndex(HiddenIndex, InputIndex, InputDimension)];
		}
		Output[InputIndex] = Sigmoid(Accumulator);
	}
}

void UDynamicLossOptimizer::BackwardPass()
{
	const int32 InputHiddenSize = InputDimension * HiddenDimension;
	const int32 HiddenOutputSize = HiddenDimension * InputDimension;

	TArray<float> GradInputHidden;
	TArray<float> GradHiddenOutput;
	GradInputHidden.SetNumZeroed(InputHiddenSize);
	GradHiddenOutput.SetNumZeroed(HiddenOutputSize);

	for (int32 InputIndex = 0; InputIndex < InputDimension; ++InputIndex)
	{
		for (int32 HiddenIndex = 0; HiddenIndex < HiddenDimension; ++HiddenIndex)
		{
			const float HiddenValue = Hidden[HiddenIndex];
			const int32 WeightIndex = GetIndex(InputIndex, HiddenIndex, HiddenDimension);
			const float Regularizer = Lambda * WeightsInputHidden[WeightIndex] * FMath::Pow(HiddenValue * (1.0f - HiddenValue), 2);
			GradInputHidden[WeightIndex] = Input[InputIndex] * GradientHidden(HiddenValue, InputIndex) + Regularizer;
		}
	}

	for (int32 HiddenIndex = 0; HiddenIndex < HiddenDimension; ++HiddenIndex)
	{
		const float HiddenValue = Hidden[HiddenIndex];
		for (int32 InputIndex = 0; InputIndex < InputDimension; ++InputIndex)
		{
			const int32 WeightIndex = GetIndex(HiddenIndex, InputIndex, InputDimension);
			const float Regularizer = Lambda * WeightsHiddenOutput[WeightIndex] * FMath::Pow(HiddenValue * (1.0f - HiddenValue), 2);
			GradHiddenOutput[WeightIndex] = HiddenValue * (Input[InputIndex] - Output[InputIndex]) + Regularizer;
		}
	}

	UpdateWeights(WeightsInputHidden, GradInputHidden, InputDimension, HiddenDimension);
	UpdateWeights(WeightsHiddenOutput, GradHiddenOutput, HiddenDimension, InputDimension);
}

void UDynamicLossOptimizer::UpdateWeights(TArray<float>& Weights, const TArray<float>& Gradients, int32 Rows, int32 Cols)
{
	for (int32 Row = 0; Row < Rows; ++Row)
	{
		for (int32 Col = 0; Col < Cols; ++Col)
		{
			const int32 Index = GetIndex(Row, Col, Cols);
			Weights[Index] -= LearningRate * Gradients[Index];
		}
	}
}

float UDynamicLossOptimizer::Sigmoid(float X) const
{
	return 1.0f / (1.0f + FMath::Exp(-X));
}

float UDynamicLossOptimizer::GradientHidden(float HiddenValue, int32 InputIndex) const
{
	if (InputDimension <= 0 || HiddenDimension <= 0)
	{
		return 0.0f;
	}

	const int32 Index = GetIndex(InputIndex % InputDimension, 0, HiddenDimension);
	return WeightsInputHidden.IsValidIndex(Index) ? WeightsInputHidden[Index] * HiddenValue * (1.0f - HiddenValue) : 0.0f;
}

int32 UDynamicLossOptimizer::GetIndex(int32 Row, int32 Col, int32 NumCols) const
{
	return Row * NumCols + Col;
}
