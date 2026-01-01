// Copyright Epic Games, Inc.

#include "QuantumBayesianLearningComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

UQuantumBayesianLearningComponent::UQuantumBayesianLearningComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumBayesianLearningComponent::RunQuantumBayesianLearning()
{
	// Initialize amplitudes/probabilities
	Amplitudes.SetNum(NumHypotheses);
	Probabilities.SetNum(NumHypotheses);
	HypothesisBars.Reset();

	for (int32 i = 0; i < NumHypotheses; ++i)
	{
		Amplitudes[i] = InitialAmplitude;
		Probabilities[i] = Amplitudes[i] * Amplitudes[i];
	}

	InitializeVisualization();

	for (int32 Iteration = 0; Iteration < NumIterations; ++Iteration)
	{
		if (bLogUpdates)
		{
			UE_LOG(LogTemp, Log, TEXT("Iteration %d"), Iteration + 1);
		}

		const int32 ObservedHypothesis = SimulateObservation();
		UpdateProbabilities(ObservedHypothesis);
		AdjustLearningRate();

		if (bLogUpdates)
		{
			LogProbabilities(FString::Printf(TEXT("After Iteration %d"), Iteration + 1));
		}

		UpdateVisualization();
	}

	MostProbableHypothesis = 0;
	float MaxValue = Probabilities.Num() > 0 ? Probabilities[0] : 0.f;
	for (int32 i = 1; i < Probabilities.Num(); ++i)
	{
		if (Probabilities[i] > MaxValue)
		{
			MaxValue = Probabilities[i];
			MostProbableHypothesis = i;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Quantum Bayesian Learning Completed."));
	UE_LOG(LogTemp, Log, TEXT("Most Probable Hypothesis: %d"), MostProbableHypothesis);
}

int32 UQuantumBayesianLearningComponent::SimulateObservation() const
{
	float TotalProb = 0.f;
	for (float P : Probabilities)
	{
		TotalProb += P;
	}

	const float RandomValue = FMath::FRandRange(0.f, TotalProb);
	float Cumulative = 0.f;

	for (int32 i = 0; i < Probabilities.Num(); ++i)
	{
		Cumulative += Probabilities[i];
		if (RandomValue <= Cumulative)
		{
			if (bLogUpdates)
			{
				UE_LOG(LogTemp, Log, TEXT("Observed Hypothesis: %d"), i);
			}
			return i;
		}
	}

	return FMath::Max(Probabilities.Num() - 1, 0);
}

void UQuantumBayesianLearningComponent::UpdateProbabilities(int32 ObservedHypothesis)
{
	const int32 Count = Amplitudes.Num();
	if (Count == 0)
	{
		return;
	}

	for (int32 i = 0; i < Count; ++i)
	{
		if (i == ObservedHypothesis)
		{
			Amplitudes[i] += LearningRate;
		}
		else
		{
			Amplitudes[i] -= LearningRate / FMath::Max(Count - 1, 1);
		}

		Amplitudes[i] = FMath::Clamp(Amplitudes[i], 0.f, 1.f);
		Probabilities[i] = Amplitudes[i] * Amplitudes[i];
	}
}

void UQuantumBayesianLearningComponent::AdjustLearningRate()
{
	float Entropy = 0.f;
	for (float P : Probabilities)
	{
		if (P > 0.f)
		{
			Entropy -= P * FMath::Loge(P);
		}
	}

	LearningRate = FMath::Clamp(0.1f / (Entropy + 1.f), 0.01f, 0.1f);

	if (bLogUpdates)
	{
		UE_LOG(LogTemp, Log, TEXT("Adjusted Learning Rate: %f"), LearningRate);
	}
}

void UQuantumBayesianLearningComponent::LogProbabilities(const FString& Message) const
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	for (int32 i = 0; i < Probabilities.Num(); ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Hypothesis %d: Amplitude = %f, Probability = %f"), i, Amplitudes[i], Probabilities[i]);
	}
}

void UQuantumBayesianLearningComponent::InitializeVisualization()
{
	ClearVisualization();

	if (!HypothesisBarClass || !GetWorld())
	{
		return;
	}

	for (int32 i = 0; i < NumHypotheses; ++i)
	{
		const FVector Location(i * BarSpacing, 0.f, 0.f);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* Bar = GetWorld()->SpawnActor<AActor>(HypothesisBarClass, Location, FRotator::ZeroRotator, Params);
		if (Bar)
		{
			Bar->SetActorScale3D(BarScale);
			HypothesisBars.Add(Bar);
		}
	}
}

void UQuantumBayesianLearningComponent::UpdateVisualization()
{
	if (HypothesisBars.Num() != Probabilities.Num())
	{
		return;
	}

	for (int32 i = 0; i < HypothesisBars.Num(); ++i)
	{
		if (AActor* Bar = HypothesisBars[i].Get())
		{
			const FVector NewScale(BarScale.X, Probabilities[i], BarScale.Z);
			Bar->SetActorScale3D(NewScale);
		}
	}
}

void UQuantumBayesianLearningComponent::ClearVisualization()
{
	for (TWeakObjectPtr<AActor>& BarPtr : HypothesisBars)
	{
		if (BarPtr.IsValid())
		{
			BarPtr->Destroy();
		}
	}
	HypothesisBars.Reset();
}
