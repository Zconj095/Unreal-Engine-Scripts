// Copyright Epic Games, Inc.

#include "LazyUpdateManagerComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"

ULazyUpdateManagerComponent::ULazyUpdateManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULazyUpdateManagerComponent::ScheduleUpdate(const FLazyUpdateDelegate& UpdateAction)
{
	if (bUpdatePending)
	{
		return;
	}

	if (!UpdateAction.IsBound())
	{
		return;
	}

	bUpdatePending = true;
	PendingAction = UpdateAction;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &ULazyUpdateManagerComponent::ExecutePending, DelaySeconds, false);
	}
	else
	{
		// Fallback: execute immediately if no world/timer available.
		ExecutePending();
	}
}

void ULazyUpdateManagerComponent::ExecutePending()
{
	PendingAction.ExecuteIfBound();

	bUpdatePending = false;
	PendingAction.Unbind();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
}
