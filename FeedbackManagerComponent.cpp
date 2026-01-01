// Copyright Epic Games, Inc.

#include "FeedbackManagerComponent.h"

#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "TimerManager.h"

UFeedbackManagerComponent::UFeedbackManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFeedbackManagerComponent::ShowMessage(const FString& Message)
{
	if (FeedbackText)
	{
		FeedbackText->SetText(FText::FromString(Message));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FeedbackText is not assigned; cannot display message: %s"), *Message);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ClearTimerHandle);
		World->GetTimerManager().SetTimer(ClearTimerHandle, this, &UFeedbackManagerComponent::ClearMessage, ClearDelaySeconds, false);
	}
}

void UFeedbackManagerComponent::ClearMessage()
{
	if (FeedbackText)
	{
		FeedbackText->SetText(FText::GetEmpty());
	}
}
