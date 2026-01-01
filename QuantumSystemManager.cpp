#include "QuantumSystemManager.h"
#include "Math/UnrealMathUtility.h"

void UQuantumSystemManager::Initialize(int32 InNumStates)
{
	NumStates = InNumStates;
	QuantumState.SetNumZeroed(NumStates);
	TransitionMatrix.SetNumZeroed(NumStates * NumStates);
	EmissionMatrix.SetNumZeroed(NumStates * NumStates);
	RandomStream.Initialize(FPlatformTime::Cycles());
}

void UQuantumSystemManager::InitializeState(const TArray<double>& Probabilities)
{
	if (Probabilities.Num() != NumStates)
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumSystemManager: State vector size mismatch."));
		return;
	}

	for (int32 Index = 0; Index < NumStates; ++Index)
	{
		QuantumState[Index] = Probabilities[Index];
	}
}

void UQuantumSystemManager::ApplyTransition()
{
	if (NumStates == 0)
	{
		return;
	}

	const TArray<double> Result = MultiplyMatrices(TransitionMatrix, QuantumState, NumStates, NumStates, 1);
	for (int32 Index = 0; Index < NumStates; ++Index)
	{
		QuantumState[Index] = Result[Index];
	}
}

int32 UQuantumSystemManager::Measure() const
{
	if (NumStates == 0)
	{
		return INDEX_NONE;
	}

	TArray<double> Probabilities;
	Probabilities.SetNumZeroed(NumStates);
	for (int32 Index = 0; Index < NumStates; ++Index)
	{
		Probabilities[Index] = QuantumState[Index] * QuantumState[Index];
	}

	return static_cast<int32>(SampleDistribution(Probabilities));
}

void UQuantumSystemManager::SetTransitionMatrix(const TArray<double>& Matrix, int32 InNumStates)
{
	if (Matrix.Num() != InNumStates * InNumStates || InNumStates != NumStates)
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumSystemManager: Transition matrix mismatch."));
		return;
	}

	TransitionMatrix = Matrix;
}

void UQuantumSystemManager::SetEmissionMatrix(const TArray<double>& Matrix, int32 InNumStates)
{
	if (Matrix.Num() != InNumStates * InNumStates || InNumStates != NumStates)
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumSystemManager: Emission matrix mismatch."));
		return;
	}

	EmissionMatrix = Matrix;
}

double UQuantumSystemManager::SampleDistribution(const TArray<double>& Probabilities) const
{
	double Total = 0.0;
	for (double Value : Probabilities)
	{
		Total += Value;
	}

	// Guard zero-sum
	if (Total <= KINDA_SMALL_NUMBER)
	{
		return 0;
	}

	const double Threshold = RandomStream.FRandRange(0.f, Total);
	double Cumulative = 0.0;
	for (int32 Index = 0; Index < Probabilities.Num(); ++Index)
	{
		Cumulative += Probabilities[Index];
		if (Threshold <= Cumulative)
		{
			return Index;
		}
	}

	return Probabilities.Num() - 1;
}

TArray<double> UQuantumSystemManager::MultiplyMatrices(const TArray<double>& A, const TArray<double>& B, int32 RowsA, int32 ColsA, int32 ColsB)
{
	TArray<double> Result;
	Result.AddZeroed(RowsA * ColsB);

	for (int32 Row = 0; Row < RowsA; ++Row)
	{
		for (int32 Col = 0; Col < ColsB; ++Col)
		{
			double Value = 0.0;
			for (int32 K = 0; K < ColsA; ++K)
			{
				Value += A[Row * ColsA + K] * B[K * ColsB + Col];
			}
			Result[Row * ColsB + Col] = Value;
		}
	}

	return Result;
}
