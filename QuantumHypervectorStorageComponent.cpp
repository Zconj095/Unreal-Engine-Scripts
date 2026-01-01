#include "QuantumHypervectorStorageComponent.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/ScopeLock.h"

UQuantumHypervectorStorageComponent::UQuantumHypervectorStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	RamdiskPath = TEXT("R:/QuantumAI_Hypervectors/");
}

void UQuantumHypervectorStorageComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeRAMDiskStorage();
}

void UQuantumHypervectorStorageComponent::InitializeRAMDiskStorage()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*RamdiskPath))
	{
		PlatformFile.CreateDirectoryTree(*RamdiskPath);
		UE_LOG(LogTemp, Log, TEXT("Quantum RAMDisk storage initialized: %s"), *RamdiskPath);
	}
}

bool UQuantumHypervectorStorageComponent::HypervectorExists(int32 LayerID) const
{
	const FString FilePath = FPaths::Combine(*RamdiskPath, FString::Printf(TEXT("Layer_%d.qhv"), LayerID));
	return FPaths::FileExists(FilePath);
}

void UQuantumHypervectorStorageComponent::DeleteHypervectorFromRAM(int32 LayerID)
{
	const FString FilePath = FPaths::Combine(*RamdiskPath, FString::Printf(TEXT("Layer_%d.qhv"), LayerID));
	if (FPaths::FileExists(FilePath))
	{
		IFileManager::Get().Delete(*FilePath);
		UE_LOG(LogTemp, Log, TEXT("Deleted hypervector for layer %d from RAM disk."), LayerID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumHypervectorStorage: No hypervector to delete for layer %d"), LayerID);
	}
}

void UQuantumHypervectorStorageComponent::StoreHypervectorInRAM(int32 LayerID, const TArray<float>& Hypervector)
{
	const FString FilePath = FPaths::Combine(*RamdiskPath, FString::Printf(TEXT("Layer_%d.qhv"), LayerID));
	TArray<FString> Lines;
	for (float Value : Hypervector)
	{
		Lines.Add(FString::SanitizeFloat(Value));
	}

	if (FFileHelper::SaveStringArrayToFile(Lines, *FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Stored hypervector for layer %d in RAM disk at %s"), LayerID, *FilePath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error storing hypervector for layer %d"), LayerID);
	}
}

TArray<float> UQuantumHypervectorStorageComponent::RetrieveHypervectorFromRAM(int32 LayerID) const
{
	const FString FilePath = FPaths::Combine(*RamdiskPath, FString::Printf(TEXT("Layer_%d.qhv"), LayerID));
	TArray<float> Result;
	TArray<FString> Lines;

	if (!FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumHypervectorStorage: No hypervector found for layer %d"), LayerID);
		return Result;
	}

	for (const FString& Line : Lines)
	{
		Result.Add(FCString::Atof(*Line));
	}

	UE_LOG(LogTemp, Log, TEXT("Loaded hypervector for layer %d from RAM disk."), LayerID);
	return Result;
}
