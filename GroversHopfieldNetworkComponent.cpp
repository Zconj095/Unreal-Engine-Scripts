// Copyright Epic Games, Inc.

#include "GroversHopfieldNetworkComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UGroversHopfieldNetworkComponent::UGroversHopfieldNetworkComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGroversHopfieldNetworkComponent::TrainNetwork()
{
	if (TrainingPatterns.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No training patterns provided."));
		return;
	}

	WeightMatrix.Reset();
	WeightMatrix.Init(0.0f, NumNeurons * NumNeurons);

	for (const FHopfieldPattern& Pattern : TrainingPatterns)
	{
		if (Pattern.Values.Num() != NumNeurons)
		{
			UE_LOG(LogTemp, Warning, TEXT("Skipping pattern with mismatched length (expected %d, got %d)."), NumNeurons, Pattern.Values.Num());
			continue;
		}

		for (int32 i = 0; i < NumNeurons; ++i)
		{
			for (int32 j = 0; j < NumNeurons; ++j)
			{
				if (i == j)
				{
					continue;
				}
				const int32 Index = i * NumNeurons + j;
				WeightMatrix[Index] += Pattern.Values[i] * Pattern.Values[j];
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Training Completed. Weight Matrix Initialized."));
}

void UGroversHopfieldNetworkComponent::SetCurrentPattern(const TArray<int32>& InPattern)
{
	if (InPattern.Num() != NumNeurons)
	{
		UE_LOG(LogTemp, Error, TEXT("Current input pattern is not set or invalid (expected %d, got %d)."), NumNeurons, InPattern.Num());
		return;
	}

	CurrentPattern = InPattern;
}

void UGroversHopfieldNetworkComponent::RetrievePattern()
{
	if (CurrentPattern.Num() != NumNeurons)
	{
		UE_LOG(LogTemp, Error, TEXT("Current input pattern is not set or invalid."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Starting Pattern Retrieval..."));
	const TArray<int32> RetrievedPattern = QuantumSearch(CurrentPattern);

	UE_LOG(LogTemp, Log, TEXT("Pattern Retrieval Completed."));
	FString RetrievedStr;
	for (int32 v : RetrievedPattern)
	{
		RetrievedStr += FString::Printf(TEXT("%d "), v);
	}
	UE_LOG(LogTemp, Log, TEXT("Retrieved Pattern: %s"), *RetrievedStr);

	VisualizePattern(RetrievedPattern);
}

TArray<int32> UGroversHopfieldNetworkComponent::QuantumSearch(const TArray<int32>& InputPattern)
{
	TArray<int32> State = InputPattern;

	for (int32 Iteration = 0; Iteration < MaxIterations; ++Iteration)
	{
		if (bLogSteps)
		{
			FString StateStr;
			for (int32 v : State)
			{
				StateStr += FString::Printf(TEXT("%d "), v);
			}
			UE_LOG(LogTemp, Log, TEXT("Iteration %d: Current State - %s"), Iteration + 1, *StateStr);
		}

		State = ApplyGroversSearch(State);

		if (IsConverged(State))
		{
			if (bLogSteps)
			{
				UE_LOG(LogTemp, Log, TEXT("Convergence Achieved."));
			}
			break;
		}
	}

	return State;
}

TArray<int32> UGroversHopfieldNetworkComponent::ApplyGroversSearch(const TArray<int32>& State)
{
	TArray<int32> NewState;
	NewState.SetNum(NumNeurons);

	TArray<float> Probabilities;
	Probabilities.SetNum(TrainingPatterns.Num());

	for (int32 i = 0; i < TrainingPatterns.Num(); ++i)
	{
		if (TrainingPatterns[i].Values.Num() != NumNeurons)
		{
			Probabilities[i] = -FLT_MAX; // invalid pattern
			continue;
		}
		Probabilities[i] = CalculateOverlap(State, TrainingPatterns[i].Values);
	}

	for (int32 Iter = 0; Iter < QuantumIterations; ++Iter)
	{
		const int32 TargetIndex = FindMaxIndex(Probabilities);

		float Mean = 0.f;
		for (float P : Probabilities)
		{
			Mean += P;
		}
		Mean /= (Probabilities.Num() > 0 ? Probabilities.Num() : 1);

		for (float& P : Probabilities)
		{
			P = 2.f * Mean - P;
		}

		// Marking step is implicit in picking max; nothing else needed in this stub loop.
		(void)TargetIndex;
	}

	const int32 MostProbableIndex = FindMaxIndex(Probabilities);
	if (TrainingPatterns.IsValidIndex(MostProbableIndex) && TrainingPatterns[MostProbableIndex].Values.Num() == NumNeurons)
	{
		NewState = TrainingPatterns[MostProbableIndex].Values;
	}

	return NewState;
}

float UGroversHopfieldNetworkComponent::CalculateOverlap(const TArray<int32>& State, const TArray<int32>& Pattern) const
{
	if (State.Num() != NumNeurons || Pattern.Num() != NumNeurons)
	{
		return -FLT_MAX;
	}

	float Overlap = 0.f;
	for (int32 i = 0; i < NumNeurons; ++i)
	{
		Overlap += State[i] * Pattern[i];
	}
	return Overlap / static_cast<float>(NumNeurons);
}

bool UGroversHopfieldNetworkComponent::IsConverged(const TArray<int32>& State) const
{
	for (const FHopfieldPattern& Pattern : TrainingPatterns)
	{
		if (Pattern.Values.Num() != NumNeurons)
		{
			continue;
		}

		bool bMatch = true;
		for (int32 i = 0; i < NumNeurons; ++i)
		{
			if (State[i] != Pattern.Values[i])
			{
				bMatch = false;
				break;
			}
		}

		if (bMatch)
		{
			return true;
		}
	}

	return false;
}

int32 UGroversHopfieldNetworkComponent::FindMaxIndex(const TArray<float>& Array) const
{
	if (Array.Num() == 0)
	{
		return 0;
	}

	int32 MaxIndex = 0;
	float MaxValue = Array[0];

	for (int32 i = 1; i < Array.Num(); ++i)
	{
		if (Array[i] > MaxValue)
		{
			MaxValue = Array[i];
			MaxIndex = i;
		}
	}

	return MaxIndex;
}

void UGroversHopfieldNetworkComponent::VisualizePattern(const TArray<int32>& Pattern)
{
	if (!NeuronActorClass || !GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Visualization components are not set."));
		return;
	}

	if (bClearPreviousVisualization)
	{
		ClearVisualization();
	}

	for (int32 i = 0; i < Pattern.Num(); ++i)
	{
		const FVector Location(i * NeuronSpacing, 0.f, 0.f);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* Neuron = GetWorld()->SpawnActor<AActor>(NeuronActorClass, Location, FRotator::ZeroRotator, Params);
		if (Neuron)
		{
			if (VisualizationRoot)
			{
				Neuron->AttachToActor(VisualizationRoot, FAttachmentTransformRules::KeepRelativeTransform);
			}
#if WITH_EDITOR
			Neuron->SetActorLabel(FString::Printf(TEXT("Neuron %d: %d"), i, Pattern[i]));
#endif
			NeuronActors.Add(Neuron);
		}
	}
}

void UGroversHopfieldNetworkComponent::ClearVisualization()
{
	for (TWeakObjectPtr<AActor>& ActorPtr : NeuronActors)
	{
		if (ActorPtr.IsValid())
		{
			ActorPtr->Destroy();
		}
	}
	NeuronActors.Reset();
}
