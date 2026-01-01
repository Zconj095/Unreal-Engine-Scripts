#include "CrystalDatabaseAsset.h"

bool UCrystalDatabaseAsset::FindCrystalByName(const FString& Name, FCrystalData& OutData) const
{
	for (const FCrystalData& Entry : Crystals)
	{
		if (Entry.CrystalName.Equals(Name, ESearchCase::IgnoreCase))
		{
			OutData = Entry;
			return true;
		}
	}
	return false;
}

TArray<FCrystalData> UCrystalDatabaseAsset::FindCrystalsByTag(const FString& Tag) const
{
	TArray<FCrystalData> Result;
	for (const FCrystalData& Entry : Crystals)
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
