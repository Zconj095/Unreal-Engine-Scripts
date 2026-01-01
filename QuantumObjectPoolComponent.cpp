#include "QuantumObjectPoolComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

UQuantumObjectPoolComponent::UQuantumObjectPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!PoolActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumObjectPoolComponent requires a PoolActorClass to spawn."));
		return;
	}

	HypervectorManager = GetOwner()->FindComponentByClass<UQuantumHypervectorManagerComponent>();
	const int32 PoolCount = HypervectorManager ? HypervectorManager->GetHypervectorCount() : DefaultPoolSize;
	InitializePool(PoolCount);
}

void UQuantumObjectPoolComponent::InitializePool(int32 Count)
{
	if (Count <= 0)
	{
		return;
	}

	if (Pool.Num() > 0)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (int32 Index = 0; Index < Count; ++Index)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* NewActor = World->SpawnActor<AActor>(PoolActorClass, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (NewActor)
		{
			NewActor->SetActorEnableCollision(false);
			NewActor->SetActorHiddenInGame(true);
			Pool.Add(NewActor);
		}
	}
}

AActor* UQuantumObjectPoolComponent::GetPooledObject()
{
	for (int32 Index = 0; Index < Pool.Num(); ++Index)
	{
		AActor* Actor = Pool[Index];
		if (Actor && Actor->IsHidden())
		{
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
			return Actor;
		}
	}

	return nullptr;
}

void UQuantumObjectPoolComponent::ReturnPooledObject(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
}
