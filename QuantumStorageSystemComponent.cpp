#include "QuantumStorageSystemComponent.h"

UQuantumStorageSystemComponent::UQuantumStorageSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<FString> UQuantumStorageSystemComponent::GetStoredFiles() const
{
	return StoredFiles.Array();
}

bool UQuantumStorageSystemComponent::FileExists(const FString& Filename) const
{
	return StoredFiles.Contains(Filename);
}

void UQuantumStorageSystemComponent::AddOrUpdateTreeSizeRecord(const FString& Filename, const FTreeSizeRecord& Record)
{
	StoredFiles.Add(Filename);
	TreeSizeData.FindOrAdd(Filename) = Record;
}
