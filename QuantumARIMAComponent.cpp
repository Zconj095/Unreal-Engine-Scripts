// Copyright Epic Games, Inc.

#include "QuantumARIMAComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

UQuantumARIMAComponent::UQuantumARIMAComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	if (TimeSeries.Num() == 0)
	{
		TimeSeries = {1.f, 2.f, 3.f, 5.f, 8.f, 13.f, 21.f};
	}
}

void UQuantumARIMAComponent::RunQuantumARIMA()
{
	RecalculatePredictions();
	VisualizePredictions();
}

void UQuantumARIMAComponent::RecalculatePredictions()
{
	const TArray<float> Differenced = PerformDifferencing(TimeSeries, D);
	Predictions = QuantumARIMAPredict(Differenced, P, Q);
}

void UQuantumARIMAComponent::ClearVisualization()
{
	for (TWeakObjectPtr<AActor>& ActorPtr : SpawnedVisualization)
	{
		if (ActorPtr.IsValid())
		{
			ActorPtr->Destroy();
		}
	}
	SpawnedVisualization.Reset();
}

TArray<float> UQuantumARIMAComponent::PerformDifferencing(const TArray<float>& Data, int32 Degree) const
{
	TArray<float> Result = Data;
	for (int32 i = 0; i < Degree; ++i)
	{
		for (int32 j = Result.Num() - 1; j > 0; --j)
		{
			Result[j] -= Result[j - 1];
		}
		if (Result.Num() > 0)
		{
			Result.RemoveAt(0);
		}
	}
	return Result;
}

TArray<float> UQuantumARIMAComponent::QuantumARIMAPredict(const TArray<float>& Data, int32 AROrder, int32 MAOrder) const
{
	TArray<float> Forecast;
	const int32 Total = Data.Num() + AROrder;
	Forecast.Reserve(Total);

	for (int32 i = 0; i < Total; ++i)
	{
		const float ARComp = ARComponent(Data, Forecast, AROrder);
		const float MAComp = MAComponent(Data, i, MAOrder);
		const float QuantumAdj = SimulateQuantumAdjustment();

		Forecast.Add(ARComp + MAComp + QuantumAdj);
	}

	return Forecast;
}

float UQuantumARIMAComponent::ARComponent(const TArray<float>& Data, const TArray<float>& Forecast, int32 Order) const
{
	float Result = 0.f;
	for (int32 i = 1; i <= Order; ++i)
	{
		const int32 Index = Forecast.Num() - i;
		if (Forecast.IsValidIndex(Index))
		{
			Result += 0.5f * Forecast[Index];
		}
	}
	return Result;
}

float UQuantumARIMAComponent::MAComponent(const TArray<float>& Data, int32 CurrentIndex, int32 Order) const
{
	float Result = 0.f;
	for (int32 i = 1; i <= Order; ++i)
	{
		const int32 DataIndex = Data.Num() - i;
		if (Data.IsValidIndex(DataIndex))
		{
			Result += 0.3f * Data[DataIndex];
		}
	}
	return Result;
}

float UQuantumARIMAComponent::SimulateQuantumAdjustment() const
{
	float Accumulated = 0.f;
	for (int32 i = 0; i < QuantumIterations; ++i)
	{
		const float Phase = FMath::FRandRange(0.f, PI * 2.f);
		Accumulated += QuantumAmplitude * FMath::Sin(Phase);
	}
	return (QuantumIterations > 0) ? Accumulated / static_cast<float>(QuantumIterations) : 0.f;
}

void UQuantumARIMAComponent::VisualizePredictions()
{
	if (!VisualizationActorClass || !GetWorld())
	{
		return;
	}

	ClearVisualization();

	const float StartX = -static_cast<float>(TimeSeries.Num()) * VisualizationSpacing * 0.5f;

	for (int32 i = 0; i < Predictions.Num(); ++i)
	{
		const FVector Location(StartX + i * VisualizationSpacing, Predictions[i], 0.f);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* SphereActor = GetWorld()->SpawnActor<AActor>(VisualizationActorClass, Location, FRotator::ZeroRotator, Params);
		if (SphereActor)
		{
			SphereActor->SetActorScale3D(FVector(VisualizationScale));
			SpawnedVisualization.Add(SphereActor);
		}
	}
}
