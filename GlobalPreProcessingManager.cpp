#include "GlobalPreProcessingManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"

AGlobalPreProcessingManager::AGlobalPreProcessingManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGlobalPreProcessingManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeMatrix();
	PopulateMatrix();
	NormalizeScores();
	OptimizePreProcessingData();

	if (bLogMatrix)
	{
		LogMatrix();
	}
}

void AGlobalPreProcessingManager::InitializeMatrix()
{
	MatrixData.Reset();
	MatrixData.SetNumZeroed(MatrixRows * MatrixColumns);
}

void AGlobalPreProcessingManager::PopulateMatrix()
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

		FPreProcessingData& Data = MatrixData[Index.X * MatrixColumns + Index.Y];
		Data.Entity = Entity;
		Data.bIsPreAllocated = false;
		Data.OptimizationScore = FMath::FRandRange(0.0f, 1.0f);
		Data.MatrixIndex = Index;
	}
}

FIntPoint AGlobalPreProcessingManager::GetAvailableIndex() const
{
	for (int32 Row = 0; Row < MatrixRows; ++Row)
	{
		for (int32 Col = 0; Col < MatrixColumns; ++Col)
		{
			const int32 Index = Row * MatrixColumns + Col;
			if (!MatrixData[Index].Entity)
			{
				return FIntPoint(Row, Col);
			}
		}
	}
	return FIntPoint(-1, -1);
}

void AGlobalPreProcessingManager::NormalizeScores()
{
	float MaxScore = 0.0f;
	for (const FPreProcessingData& Data : MatrixData)
	{
		if (Data.Entity && Data.OptimizationScore > MaxScore)
		{
			MaxScore = Data.OptimizationScore;
		}
	}

	if (MaxScore <= 0.0f)
	{
		return;
	}

	for (FPreProcessingData& Data : MatrixData)
	{
		if (Data.Entity)
		{
			Data.OptimizationScore = (Data.OptimizationScore / MaxScore) * NormalizationFactor;
		}
	}
}

void AGlobalPreProcessingManager::OptimizePreProcessingData()
{
	for (FPreProcessingData& Data : MatrixData)
	{
		if (!Data.Entity)
		{
			continue;
		}

		const FVector SceneCenter = FVector::ZeroVector;
		const float Distance = FVector::Dist(Data.Entity->GetActorLocation(), SceneCenter);
		Data.OptimizationScore += FMath::Max(0.0f, NormalizationFactor / (Distance + 1.0f));
	}
}

void AGlobalPreProcessingManager::LogMatrix() const
{
	UE_LOG(LogTemp, Log, TEXT("Global Preprocessing Matrix:"));

	for (int32 Row = 0; Row < MatrixRows; ++Row)
	{
		FString RowString;
		for (int32 Col = 0; Col < MatrixColumns; ++Col)
		{
			const int32 Index = Row * MatrixColumns + Col;
			const FPreProcessingData& Data = MatrixData[Index];
			if (Data.Entity)
			{
				RowString += FString::Printf(TEXT("[%s (%.2f)] "), *Data.Entity->GetName(), Data.OptimizationScore);
			}
			else
			{
				RowString += TEXT("[EMPTY] ");
			}
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), *RowString);
	}
}
