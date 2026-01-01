#include "GlobalPreAllocationManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"

AGlobalPreAllocationManager::AGlobalPreAllocationManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGlobalPreAllocationManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeMatrix();
	PopulateMatrix();
	OptimizeAllocations();

	if (bLogMatrix)
	{
		LogMatrix();
	}
}

void AGlobalPreAllocationManager::InitializeMatrix()
{
	AllocationMatrix.Reset();
	AllocationMatrix.SetNumZeroed(MatrixRows * MatrixColumns);
}

void AGlobalPreAllocationManager::PopulateMatrix()
{
	TArray<AActor*> Entities;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Entities);

	for (AActor* Entity : Entities)
	{
		if (!Entity || Entity->Tags.Contains(PreAllocatedTag))
		{
			continue;
		}

		if (!Entity->FindComponentByClass<UPrimitiveComponent>())
		{
			continue;
		}

		const FIntPoint Index = GetAvailableIndex();
		if (Index.X < 0 || Index.Y < 0)
		{
			break;
		}

		FGlobalAllocationData& Data = AllocationMatrix[Index.X * MatrixColumns + Index.Y];
		Data.Entity = Entity;
		Data.bIsPreAllocated = false;
		Data.AllocationScore = DefaultAllocationScore;
		Data.MatrixIndex = Index;
	}
}

FIntPoint AGlobalPreAllocationManager::GetAvailableIndex() const
{
	for (int32 Row = 0; Row < MatrixRows; ++Row)
	{
		for (int32 Col = 0; Col < MatrixColumns; ++Col)
		{
			const int32 Index = Row * MatrixColumns + Col;
			if (!AllocationMatrix[Index].Entity)
			{
				return FIntPoint(Row, Col);
			}
		}
	}
	return FIntPoint(-1, -1);
}

void AGlobalPreAllocationManager::OptimizeAllocations()
{
	for (FGlobalAllocationData& Data : AllocationMatrix)
	{
		if (!Data.Entity)
		{
			continue;
		}

		const FVector SceneCenter = FVector::ZeroVector;
		const float Distance = FVector::Dist(Data.Entity->GetActorLocation(), SceneCenter);
		Data.AllocationScore = FMath::Max(1.0f, 100.0f / (Distance + 1.0f));
	}
}

void AGlobalPreAllocationManager::LogMatrix() const
{
	UE_LOG(LogTemp, Log, TEXT("Global Pre-Allocation Matrix:"));

	for (int32 Row = 0; Row < MatrixRows; ++Row)
	{
		FString RowString;
		for (int32 Col = 0; Col < MatrixColumns; ++Col)
		{
			const int32 Index = Row * MatrixColumns + Col;
			const FGlobalAllocationData& Data = AllocationMatrix[Index];

			if (Data.Entity)
			{
				RowString += FString::Printf(TEXT("[%s (%.2f)] "), *Data.Entity->GetName(), Data.AllocationScore);
			}
			else
			{
				RowString += TEXT("[EMPTY] ");
			}
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), *RowString);
	}
}
