#include "MainThreadDispatcherComponent.h"
#include "Misc/ScopeLock.h"
#include "Templates/Function.h"

TQueue<TFunction<void()>, EQueueMode::Mpsc> UMainThreadDispatcherComponent::PendingActions;
FCriticalSection UMainThreadDispatcherComponent::QueueCriticalSection;
UMainThreadDispatcherComponent* UMainThreadDispatcherComponent::SingletonInstance = nullptr;

UMainThreadDispatcherComponent::UMainThreadDispatcherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMainThreadDispatcherComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!SingletonInstance)
	{
		SingletonInstance = this;
	}
	else
	{
		SetComponentTickEnabled(false);
	}
}

void UMainThreadDispatcherComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (SingletonInstance == this)
	{
		SingletonInstance = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void UMainThreadDispatcherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SingletonInstance != this)
	{
		return;
	}

	TFunction<void()> Action;
	while (true)
	{
		{
			FScopeLock Lock(&QueueCriticalSection);
			if (!PendingActions.Dequeue(Action))
			{
				break;
			}
		}

		if (Action)
		{
			Action();
		}
	}
}

void UMainThreadDispatcherComponent::Enqueue(TFunction<void()> InAction)
{
	if (!InAction)
	{
		return;
	}

	FScopeLock Lock(&QueueCriticalSection);
	PendingActions.Enqueue(MoveTemp(InAction));
}
