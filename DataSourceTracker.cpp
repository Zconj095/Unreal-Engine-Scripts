#include "DataSourceTracker.h"

void FDataSourceTracker::AddSource(const FString& DataId, const FString& Source)
{
    SourceMetadata.Add(DataId, Source);
}

FString FDataSourceTracker::GetSource(const FString& DataId) const
{
    if (const FString* Found = SourceMetadata.Find(DataId))
    {
        return *Found;
    }
    return TEXT("Unknown Source");
}

