#include "DatabaseRecoveryCenter.h"

#include "Engine/World.h"
#include "TimerManager.h"

ADatabaseRecoveryCenter::ADatabaseRecoveryCenter()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void ADatabaseRecoveryCenter::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Database Recovery Center Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            StoreDataTimerHandle,
            this,
            &ADatabaseRecoveryCenter::HandleStoreDataTick,
            FMath::Max(0.0f, StoreDataInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveDataTimerHandle,
            this,
            &ADatabaseRecoveryCenter::HandleRetrieveDataTick,
            FMath::Max(0.0f, RetrieveDataInterval),
            true);

        World->GetTimerManager().SetTimer(
            ScanVirusTimerHandle,
            this,
            &ADatabaseRecoveryCenter::HandleScanVirusTick,
            FMath::Max(0.0f, ScanVirusInterval),
            true);

        World->GetTimerManager().SetTimer(
            BarrierToggleTimerHandle,
            this,
            &ADatabaseRecoveryCenter::HandleBarrierToggleTick,
            FMath::Max(0.0f, BarrierToggleInterval),
            true);

        World->GetTimerManager().SetTimer(
            FrequencyAccessTimerHandle,
            this,
            &ADatabaseRecoveryCenter::HandleFrequencyAccessTick,
            FMath::Max(0.0f, FrequencyAccessInterval),
            true);
    }
}

void ADatabaseRecoveryCenter::StoreData(const FString& Filename, const FString& Data)
{
    if (bIsBarrierActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Barrier is active. Cannot store data."));
        return;
    }

    const FString CompressedData = CompressData(Data);
    CompressedGhostFiles.Add(Filename);
    GalacticStorage.Add(Filename, CompressedData);

    UE_LOG(LogTemp, Log, TEXT("Stored data: %s, Compressed."), *Filename);
}

FString ADatabaseRecoveryCenter::RetrieveData(const FString& Filename)
{
    if (const FString* Found = GalacticStorage.Find(Filename))
    {
        const FString DecompressedData = DecompressData(*Found);
        UE_LOG(LogTemp, Log, TEXT("Retrieved and decompressed data: %s"), *Filename);
        return DecompressedData;
    }
    UE_LOG(LogTemp, Warning, TEXT("File '%s' not found in storage."), *Filename);
    return FString();
}

FString ADatabaseRecoveryCenter::CompressData(const FString& Data) const
{
    // Simulate compression by reversing string
    const int32 Len = Data.Len();
    FString Out;
    Out.Reserve(Len);
    for (int32 i = Len - 1; i >= 0; --i)
    {
        Out.AppendChar(Data[i]);
    }
    return Out;
}

FString ADatabaseRecoveryCenter::DecompressData(const FString& CompressedData) const
{
    // Reverse again to simulate decompression
    const int32 Len = CompressedData.Len();
    FString Out;
    Out.Reserve(Len);
    for (int32 i = Len - 1; i >= 0; --i)
    {
        Out.AppendChar(CompressedData[i]);
    }
    return Out;
}

bool ADatabaseRecoveryCenter::ScanForViruses(const FString& Data)
{
    const bool bHasVirus = Data.Contains(TEXT("virus"));
    if (bHasVirus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Virus detected in data!"));
        return false;
    }

    VirusScannedFiles.Add(Data);
    UE_LOG(LogTemp, Log, TEXT("Data is clean."));
    return true;
}

void ADatabaseRecoveryCenter::ActivateBarrier()
{
    bIsBarrierActive = true;
    UE_LOG(LogTemp, Log, TEXT("Barrier activated. Data input restricted."));
}

void ADatabaseRecoveryCenter::DeactivateBarrier()
{
    bIsBarrierActive = false;
    UE_LOG(LogTemp, Log, TEXT("Barrier deactivated. Data input unrestricted."));
}

void ADatabaseRecoveryCenter::AccessFrequencyChamber(float Frequency)
{
    if (FMath::IsNearlyEqual(Frequency, AdaptationFrequency, KINDA_SMALL_NUMBER))
    {
        UE_LOG(LogTemp, Log, TEXT("Frequency Chamber accessed. Accepting commands."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid frequency for chamber access."));
    }
}

// Timers
void ADatabaseRecoveryCenter::HandleStoreDataTick()
{
    StoreData(TEXT("example.txt"), TEXT("This is some example data."));
}

void ADatabaseRecoveryCenter::HandleRetrieveDataTick()
{
    RetrieveData(TEXT("example.txt"));
}

void ADatabaseRecoveryCenter::HandleScanVirusTick()
{
    ScanForViruses(TEXT("This data is clean."));
}

void ADatabaseRecoveryCenter::HandleBarrierToggleTick()
{
    if (bIsBarrierActive)
    {
        DeactivateBarrier();
    }
    else
    {
        ActivateBarrier();
    }
}

void ADatabaseRecoveryCenter::HandleFrequencyAccessTick()
{
    AccessFrequencyChamber(AdaptationFrequency);
}

