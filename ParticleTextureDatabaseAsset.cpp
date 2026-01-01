#include "ParticleTextureDatabaseAsset.h"

bool UParticleTextureDatabaseAsset::FindTextureByName(const FString& Name, FParticleTextureData& OutData) const
{
	for (const FParticleTextureData& E : Textures)
	{
		if (E.TextureName.Equals(Name, ESearchCase::IgnoreCase))
		{
			OutData = E;
			return true;
		}
	}
	return false;
}

TArray<FParticleTextureData> UParticleTextureDatabaseAsset::FindTexturesByTag(const FString& Tag) const
{
	TArray<FParticleTextureData> Result;
	for (const FParticleTextureData& E : Textures)
	{
		for (const FString& T : E.Tags)
		{
			if (T.Equals(Tag, ESearchCase::IgnoreCase))
			{
				Result.Add(E);
				break;
			}
		}
	}
	return Result;
}

bool UParticleTextureDatabaseAsset::AddTexture(const FParticleTextureData& Texture)
{
	const bool bExists = Textures.ContainsByPredicate([&Texture](const FParticleTextureData& Existing)
	{
		return Existing.TextureName.Equals(Texture.TextureName, ESearchCase::IgnoreCase);
	});
	if (bExists)
	{
		return false;
	}
	Textures.Add(Texture);
	return true;
}

bool UParticleTextureDatabaseAsset::RemoveTextureByName(const FString& Name)
{
	const int32 Index = Textures.IndexOfByPredicate([&Name](const FParticleTextureData& Existing)
	{
		return Existing.TextureName.Equals(Name, ESearchCase::IgnoreCase);
	});
	if (Index != INDEX_NONE)
	{
		Textures.RemoveAt(Index);
		return true;
	}
	return false;
}
