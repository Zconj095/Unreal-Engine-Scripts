#include "AssetExplorerLibrary.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"

void UAssetExplorerLibrary::ListAssets(TArray<FString>& OutAssetPaths, bool bOnlyOnDiskAssets)
{
    OutAssetPaths.Reset();

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    TArray<FAssetData> AllAssets;
    AssetRegistry.GetAllAssets(AllAssets, bOnlyOnDiskAssets);

    OutAssetPaths.Reserve(AllAssets.Num());
    for (const FAssetData& Asset : AllAssets)
    {
        OutAssetPaths.Add(Asset.GetObjectPathString());
    }
}

void UAssetExplorerLibrary::LogAssets(bool bOnlyOnDiskAssets)
{
    TArray<FString> Paths;
    ListAssets(Paths, bOnlyOnDiskAssets);
    for (const FString& Path : Paths)
    {
        UE_LOG(LogTemp, Log, TEXT("Asset: %s"), *Path);
    }
}
