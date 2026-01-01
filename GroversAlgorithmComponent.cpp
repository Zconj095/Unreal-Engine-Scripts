// Copyright Epic Games, Inc.

#include "GroversAlgorithmComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UGroversAlgorithmComponent::UGroversAlgorithmComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UGroversAlgorithmComponent::RunGrover()
{
	if (NumQubits < 1 || TargetState >= (1 << NumQubits))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid settings for Grover's Algorithm. Check NumQubits and TargetState."));
		return FVector::ZeroVector;
	}

	UE_LOG(LogTemp, Log, TEXT("Running Grover's Algorithm with %d qubits and target state %d."), NumQubits, TargetState);

	const int32 TotalStates = 1 << NumQubits;

	Probabilities.Empty();
	Probabilities.SetNum(TotalStates);
	const float InitialProb = 1.0f / static_cast<float>(TotalStates);
	for (float& P : Probabilities)
	{
		P = InitialProb; // equal superposition
	}

	if (bEnableVisualization)
	{
		InitializeVisualization(TotalStates);
	}

	for (int32 Iter = 0; Iter < Iterations; ++Iter)
	{
		if (bLogIntermediateStates)
		{
			LogProbabilities(FString::Printf(TEXT("Before Iteration %d"), Iter + 1));
		}

		ApplyOracle();
		ApplyDiffusionOperator();

		if (bEnableVisualization)
		{
			UpdateVisualization();
		}

		if (bLogIntermediateStates)
		{
			LogProbabilities(FString::Printf(TEXT("After Iteration %d"), Iter + 1));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Grover's Algorithm Completed."));
	const int32 MostProbableState = GetMostProbableState();
	const float Probability = Probabilities.IsValidIndex(MostProbableState) ? Probabilities[MostProbableState] : 0.0f;
	UE_LOG(LogTemp, Log, TEXT("Most Probable State: %d"), MostProbableState);

	return FVector(static_cast<float>(MostProbableState), Probability, static_cast<float>(Iterations));
}

void UGroversAlgorithmComponent::ApplyOracle()
{
	if (Probabilities.IsValidIndex(TargetState))
	{
		Probabilities[TargetState] *= -1.0f;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Target state %d is out of bounds for probabilities list."), TargetState);
	}
}

void UGroversAlgorithmComponent::ApplyDiffusionOperator()
{
	if (Probabilities.Num() == 0)
	{
		return;
	}

	float MeanAmplitude = 0.0f;
	for (float P : Probabilities)
	{
		MeanAmplitude += P;
	}
	MeanAmplitude /= static_cast<float>(Probabilities.Num());

	for (float& P : Probabilities)
	{
		P = 2.0f * MeanAmplitude - P;
	}
}

int32 UGroversAlgorithmComponent::GetMostProbableState() const
{
	if (Probabilities.Num() == 0)
	{
		return 0;
	}

	int32 MaxIndex = 0;
	float MaxValue = Probabilities[0];
	for (int32 i = 1; i < Probabilities.Num(); ++i)
	{
		if (Probabilities[i] > MaxValue)
		{
			MaxValue = Probabilities[i];
			MaxIndex = i;
		}
	}
	return MaxIndex;
}

void UGroversAlgorithmComponent::LogProbabilities(const FString& Message) const
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	for (int32 i = 0; i < Probabilities.Num(); ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("State |%d>: %f"), i, Probabilities[i]);
	}
}

void UGroversAlgorithmComponent::InitializeVisualization(int32 TotalStates)
{
	if (!ProbabilityBarClass || !GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Probability visualization components are not set."));
		return;
	}

	ClearVisualization();

	ProbabilityBars.Reserve(TotalStates);

	for (int32 i = 0; i < TotalStates; ++i)
	{
		const FVector Location(i * BarSpacing, 0.f, 0.f);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if (VisualizationRoot)
		{
			Params.Owner = VisualizationRoot;
			Params.Instigator = VisualizationRoot->GetInstigator();
		}

		AActor* Bar = GetWorld()->SpawnActor<AActor>(ProbabilityBarClass, Location, FRotator::ZeroRotator, Params);
		if (Bar && VisualizationRoot)
		{
			Bar->AttachToActor(VisualizationRoot, FAttachmentTransformRules::KeepRelativeTransform);
		}
		if (Bar)
		{
#if WITH_EDITOR
			Bar->SetActorLabel(FString::Printf(TEXT("State |%d>"), i));
#endif
			ProbabilityBars.Add(Bar);
		}
	}

	if (ProbabilityBars.Num() != TotalStates)
	{
		UE_LOG(LogTemp, Error, TEXT("Number of probability bars (%d) does not match total states (%d)."), ProbabilityBars.Num(), TotalStates);
	}
}

void UGroversAlgorithmComponent::UpdateVisualization()
{
	if (ProbabilityBars.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Probability bars are not initialized."));
		return;
	}

	if (ProbabilityBars.Num() != Probabilities.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Probability bars count (%d) does not match probabilities count (%d)."), ProbabilityBars.Num(), Probabilities.Num());
		return;
	}

	for (int32 i = 0; i < Probabilities.Num(); ++i)
	{
		if (!ProbabilityBars[i].IsValid())
		{
			continue;
		}

		const float Height = FMath::Abs(Probabilities[i]);
		const FVector NewScale(BarBaseScale, BarBaseScale, Height);
		ProbabilityBars[i]->SetActorScale3D(NewScale);
	}
}

void UGroversAlgorithmComponent::ClearVisualization()
{
	for (TWeakObjectPtr<AActor>& BarPtr : ProbabilityBars)
	{
		if (BarPtr.IsValid())
		{
			BarPtr->Destroy();
		}
	}
	ProbabilityBars.Reset();
}
