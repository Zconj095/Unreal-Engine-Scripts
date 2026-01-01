#include "FlowerDatabaseAsset.h"

bool UFlowerDatabaseAsset::FindFlowerByName(const FString& Name, FFlowerData& OutData) const
{
	for (const FFlowerData& Entry : Flowers)
	{
		if (Entry.FlowerName.Equals(Name, ESearchCase::IgnoreCase))
		{
			OutData = Entry;
			return true;
		}
	}
	return false;
}

TArray<FFlowerData> UFlowerDatabaseAsset::FindFlowersByTag(const FString& Tag) const
{
	TArray<FFlowerData> Result;
	for (const FFlowerData& Entry : Flowers)
	{
		for (const FString& T : Entry.Tags)
		{
			if (T.Equals(Tag, ESearchCase::IgnoreCase))
			{
				Result.Add(Entry);
				break;
			}
		}
	}
	return Result;
}

bool UFlowerDatabaseAsset::AddFlower(const FFlowerData& Flower)
{
	const bool bExists = Flowers.ContainsByPredicate([&Flower](const FFlowerData& Existing)
	{
		return Existing.FlowerName.Equals(Flower.FlowerName, ESearchCase::IgnoreCase);
	});
	if (bExists)
	{
		return false;
	}
	Flowers.Add(Flower);
	return true;
}

bool UFlowerDatabaseAsset::RemoveFlowerByName(const FString& Name)
{
	const int32 Index = Flowers.IndexOfByPredicate([&Name](const FFlowerData& Existing)
	{
		return Existing.FlowerName.Equals(Name, ESearchCase::IgnoreCase);
	});
	if (Index != INDEX_NONE)
	{
		Flowers.RemoveAt(Index);
		return true;
	}
	return false;
}

TArray<FFlowerData> UFlowerDatabaseAsset::FindFlowersByType(const FString& Type) const
{
	TArray<FFlowerData> Result;
	for (const FFlowerData& Entry : Flowers)
	{
		if (Entry.FlowerType.Equals(Type, ESearchCase::IgnoreCase))
		{
			Result.Add(Entry);
		}
	}
	return Result;
}