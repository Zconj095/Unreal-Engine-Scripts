#include "GatedRecurrentUnitComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/OutputDeviceDebug.h"

#include <cmath>

UGatedRecurrentUnitComponent::UGatedRecurrentUnitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGatedRecurrentUnitComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeParameters();
	InitializeState();

	const FGruVector NewState = ComputeGRUStep(InputVector, PrevState);

	FString StateString;
	for (int32 Index = 0; Index < NewState.Num(); ++Index)
	{
		StateString += FString::Printf(TEXT("%.3f "), NewState[Index]);
	}

	UE_LOG(LogTemp, Log, TEXT("GRU Updated Hidden State: %s"), *StateString);
}

void UGatedRecurrentUnitComponent::InitializeParameters()
{
	Wf = FGruMatrix(StateSize, InputSize);
	Uf = FGruMatrix(StateSize, StateSize);
	Wh = FGruMatrix(StateSize, InputSize);
	Uh = FGruMatrix(StateSize, StateSize);

	auto RandomizeMatrix = [](FGruMatrix& Matrix)
	{
		for (float& Value : Matrix.Values)
		{
			Value = FMath::FRand();
		}
	};

	RandomizeMatrix(Wf);
	RandomizeMatrix(Uf);
	RandomizeMatrix(Wh);
	RandomizeMatrix(Uh);

	bf = FGruVector(StateSize);
	bh = FGruVector(StateSize);
	for (int32 Index = 0; Index < StateSize; ++Index)
	{
		bf[Index] = FMath::FRand();
		bh[Index] = FMath::FRand();
	}
}

void UGatedRecurrentUnitComponent::InitializeState()
{
	PrevState = FGruVector::Zeros(StateSize);
	InputVector = FGruVector(InputSize);

	for (int32 Index = 0; Index < InputSize; ++Index)
	{
		InputVector[Index] = FMath::FRand();
	}
}

FGruVector UGatedRecurrentUnitComponent::Sigmoid(const FGruVector& Vector) const
{
	FGruVector Result(Vector.Num());
	for (int32 Index = 0; Index < Vector.Num(); ++Index)
	{
		Result[Index] = 1.0f / (1.0f + FMath::Exp(-Vector[Index]));
	}
	return Result;
}

FGruVector UGatedRecurrentUnitComponent::Tanh(const FGruVector& Vector) const
{
	FGruVector Result(Vector.Num());
	for (int32 Index = 0; Index < Vector.Num(); ++Index)
	{
		Result[Index] = static_cast<float>(FMath::Clamp(tanh(static_cast<double>(Vector[Index])), -1.0, 1.0));
	}
	return Result;
}

FGruVector UGatedRecurrentUnitComponent::ComputeGRUStep(const FGruVector& Input, const FGruVector& PreviousState) const
{
	const FGruVector ForgetGate = Sigmoid(Wf.Multiply(Input) + Uf.Multiply(PreviousState) + bf);
	const FGruVector Candidate = Tanh(Wh.Multiply(Input) + Uh.Multiply(ForgetGate.Hadamard(PreviousState)) + bh);
	const FGruVector OneMinusForget = FGruVector::Ones(ForgetGate.Num()) - ForgetGate;

	return OneMinusForget.Hadamard(PreviousState) + ForgetGate.Hadamard(Candidate);
}
