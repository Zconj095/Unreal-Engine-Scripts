#include "QuantumHyperstateLoggerComponent.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"

UQuantumHyperstateLoggerComponent::UQuantumHyperstateLoggerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHyperstateLoggerComponent::LogPrompt(const FString& Category, const FString& Prompt, float Hyperstate)
{
	const int32 VectorTimeGap = CalculateVectorTimeGap(Hyperstate);
	const FDateTime CurrentTime = FDateTime::UtcNow();

	{
		FScopeLock Lock(&LogCriticalSection);
		FQuantumHyperstateLogEntry Entry;
		Entry.Timestamp = CurrentTime;
		Entry.Category = Category;
		Entry.Prompt = Prompt;
		Entry.Hyperstate = Hyperstate;
		Entry.VectorTimeGap = VectorTimeGap;
		LogEntries.Add(MoveTemp(Entry));
	}

	ApplyQuantumPhaseEvolution();
	TimeSeriesForecastLogging();
}

TFuture<TArray<FQuantumHyperstateLogEntry>> UQuantumHyperstateLoggerComponent::RetrieveLogsAsync(const FDateTime& StartTime, const FDateTime& EndTime)
{
	TArray<FQuantumHyperstateLogEntry> Snapshot;
	{
		FScopeLock Lock(&LogCriticalSection);
		Snapshot = LogEntries;
	}

	return Async(EAsyncExecution::ThreadPool, [Snapshot = MoveTemp(Snapshot), StartTime, EndTime]() mutable -> TArray<FQuantumHyperstateLogEntry>
	{
		TArray<FQuantumHyperstateLogEntry> Result;
		for (const FQuantumHyperstateLogEntry& Entry : Snapshot)
		{
			if (Entry.Timestamp >= StartTime && Entry.Timestamp <= EndTime)
			{
				Result.Add(Entry);
			}
		}
		return Result;
	});
}

int32 UQuantumHyperstateLoggerComponent::CalculateVectorTimeGap(float Hyperstate) const
{
	const float RandomFactor = FMath::RandRange(1.f, 10.f);
	return FMath::RoundToInt(Hyperstate * HypervectorInfluence * RandomFactor);
}

void UQuantumHyperstateLoggerComponent::ApplyQuantumPhaseEvolution()
{
	FScopeLock Lock(&LogCriticalSection);
	for (FQuantumHyperstateLogEntry& Entry : LogEntries)
	{
		const float Adjustment = Entry.VectorTimeGap * HypervectorInfluence;
		Entry.Timestamp = Entry.Timestamp + FTimespan::FromMilliseconds(Adjustment);
	}
}

void UQuantumHyperstateLoggerComponent::TimeSeriesForecastLogging()
{
	FScopeLock Lock(&LogCriticalSection);
	ForecastedLogs.Reset();

	for (const FQuantumHyperstateLogEntry& Entry : LogEntries)
	{
		FQuantumHyperstateForecast Forecast;
		Forecast.ForecastTime = Entry.Timestamp + FTimespan::FromMilliseconds(Entry.VectorTimeGap);
		Forecast.Prediction = FString::Printf(TEXT("Predicted event for %s at %s based on hyperstate %.2f."),
			*Entry.Category, *Forecast.ForecastTime.ToString(), Entry.Hyperstate);
		ForecastedLogs.Add(MoveTemp(Forecast));
	}
}
