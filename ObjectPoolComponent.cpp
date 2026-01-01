// Copyright Epic Games, Inc.

#include "ObjectPoolComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

UObjectPoolComponent::UObjectPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	PrewarmPool();
}

void UObjectPoolComponent::PrewarmPool()
{
	if (!PooledClass || !GetWorld())
	{
		return;
	}

	for (int32 i = 0; i < InitialPoolSize; ++i)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* Actor = GetWorld()->SpawnActor<AActor>(PooledClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		if (Actor)
		{
			Actor->SetActorHiddenInGame(true);
			Actor->SetActorEnableCollision(false);
			Actor->SetActorTickEnabled(false);
			Pool.Add(Actor);
		}
	}
}

AActor* UObjectPoolComponent::GetPooledObject()
{
	if (Pool.Num() == 0)
	{
		if (!PooledClass || !GetWorld())
		{
			return nullptr;
		}

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* NewActor = GetWorld()->SpawnActor<AActor>(PooledClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		return NewActor;
	}

	AActor* Actor = Pool.Last();
	Pool.Pop();

	if (Actor)
	{
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
		Actor->SetActorTickEnabled(true);
	}

	return Actor;
}

void UObjectPoolComponent::ReturnObject(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
	Pool.Add(Actor);
}
