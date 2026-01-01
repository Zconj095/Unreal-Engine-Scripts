#include "EnergyProcessingComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyProcessingComponent::UEnergyProcessingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InputSize = 5;
	OutputSize = 3;
	Epsilon = 1e-5f;
	LogOutput = TEXT("");
}

void UEnergyProcessingComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeParameters();

	TArray<float> EnergyPerceptionValues = EnergyPerception(Params.InputVector);
	TArray<float> AmplifiedEnergy = EnergyAmplification(EnergyPerceptionValues);
	TArray<float> Controlled = ControlledOutput(AmplifiedEnergy);
	TArray<float> FinalFlow = Softmax(Controlled);

	LogResults(Params.InputVector, FinalFlow);
}

void UEnergyProcessingComponent::InitializeParameters()
{
	auto RandomFloat = [&]() { return FMath::FRandRange(-1.0f, 1.0f); };

	Params.InputVector.SetNum(InputSize);
	Params.Weights.SetNum(OutputSize * InputSize);
	Params.Biases.SetNum(OutputSize);

	for (int32 Index = 0; Index < InputSize; ++Index)
	{
		Params.InputVector[Index] = RandomFloat();
	}

	for (int32 Index = 0; Index < Params.Weights.Num(); ++Index)
	{
		Params.Weights[Index] = RandomFloat();
	}

	for (int32 Index = 0; Index < Params.Biases.Num(); ++Index)
	{
		Params.Biases[Index] = RandomFloat();
	}
}

TArray<float> UEnergyProcessingComponent::EnergyPerception(const TArray<float>& Input) const
{
	TArray<float> Output;
	Output.SetNum(OutputSize);
	for (int32 Row = 0; Row < OutputSize; ++Row)
	{
		float Sum = Params.Biases[Row];
		for (int32 Column = 0; Column < InputSize; ++Column)
		{
			Sum += Params.Weights[Row * InputSize + Column] * Input[Column];
		}
		Output[Row] = FMath::Tanh(Sum);
	}
	return Output;
}

TArray<float> UEnergyProcessingComponent::EnergyAmplification(const TArray<float>& Input) const
{
	TArray<float> Output;
	Output.SetNum(Input.Num());
	for (int32 Index = 0; Index < Input.Num(); ++Index)
	{
		float Value = Input[Index];
		Output[Index] = FMath::Sign(Value) * FMath::Square(FMath::Abs(Value));
	}
	return Output;
}

TArray<float> UEnergyProcessingComponent::ControlledOutput(const TArray<float>& Input) const
{
	float MaxAbs = 0.0f;
	for (float Value : Input)
	{
		MaxAbs = FMath::Max(MaxAbs, FMath::Abs(Value));
	}
	float Scale = MaxAbs + Epsilon;
	TArray<float> Output;
	Output.SetNum(Input.Num());
	for (int32 Index = 0; Index < Input.Num(); ++Index)
	{
		Output[Index] = Input[Index] / Scale;
	}
	return Output;
}

TArray<float> UEnergyProcessingComponent::Softmax(const TArray<float>& Input) const
{
	TArray<float> Output;
	Output.SetNum(Input.Num());
	float SumExp = 0.0f;
	for (float Value : Input)
	{
		float ExpValue = FMath::Exp(Value);
		SumExp += ExpValue;
		Output.Add(ExpValue);
	}
	for (float& Value : Output)
	{
		Value /= SumExp;
	}
	return Output;
}

void UEnergyProcessingComponent::LogResults(const TArray<float>& InputVector, const TArray<float>& Result) const
{
	FString InputText = TEXT("Input Vector: ");
	for (float Value : InputVector)
	{
		InputText += FString::SanitizeFloat(Value) + TEXT(" ");
	}

	FString ResultText = TEXT("Final Energy Flow (Softmax): ");
	for (float Value : Result)
	{
		ResultText += FString::SanitizeFloat(Value) + TEXT(" ");
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *InputText);
	UE_LOG(LogTemp, Log, TEXT("%s"), *ResultText);
}
