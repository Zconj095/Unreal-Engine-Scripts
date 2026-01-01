#include "QuantumSceneOptimizerComponent.h"

#include "Engine/LevelStreamingDynamic.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UQuantumSceneOptimizerComponent::UQuantumSceneOptimizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumSceneOptimizerComponent::BeginPlay()
{
	Super::BeginPlay();

	Qubit = GetOwner()->FindComponentByClass<UQuantumOperationQubitComponent>();
	if (!Qubit)
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumSceneOptimizerComponent requires a QuantumOperationQubitComponent."));
		return;
	}

	OptimizationFactor = Qubit->Measure();
	StartAsyncLoad();
}

void UQuantumSceneOptimizerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(TickHandle);
	Super::EndPlay(EndPlayReason);
}

void UQuantumSceneOptimizerComponent::StartAsyncLoad()
{
	if (TargetLevel.IsNone())
	{
		return;
	}

	bool bLoadSuccess = false;
	StreamingLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), TargetLevel.ToString(), FVector::ZeroVector, FRotator::ZeroRotator, bLoadSuccess);
	if (!StreamingLevel.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to stream level %s."), *TargetLevel.ToString());
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(TickHandle, this, &UQuantumSceneOptimizerComponent::OnAsyncLoadTick, PollInterval, true);
}

void UQuantumSceneOptimizerComponent::OnAsyncLoadTick()
{
	if (!StreamingLevel.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TickHandle);
		return;
	}

	const float Progress = StreamingLevel->GetLevelStreamingStatus() == LEVEL_Visible ? 1.0f :
		StreamingLevel->GetLevelStreamingStatus() == LEVEL_Loaded ? 0.9f : 0.0f;

	UE_LOG(LogTemp, Log, TEXT("Streaming progress: %.2f%%"), Progress * 100.0f);

	if (Progress >= OptimizationFactor)
	{
		StreamingLevel->SetShouldBeVisible(true);
		StreamingLevel->SetShouldBeLoaded(true);
		GetWorld()->GetTimerManager().ClearTimer(TickHandle);
	}
}
