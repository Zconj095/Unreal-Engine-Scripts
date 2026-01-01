#include "MatrixCodingNetwork.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixCodingNetwork::AMatrixCodingNetwork()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixCodingNetwork::BeginPlay()
{
    Super::BeginPlay();

    InitializePaths();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddCodingDataTimerHandle,
            this,
            &AMatrixCodingNetwork::HandleAddCodingDataTick,
            FMath::Max(0.0f, AddCodingDataInterval),
            true);

        World->GetTimerManager().SetTimer(
            ProcessCodingDataTimerHandle,
            this,
            &AMatrixCodingNetwork::HandleProcessCodingDataTick,
            FMath::Max(0.0f, ProcessCodingDataInterval),
            true);

        World->GetTimerManager().SetTimer(
            ManipulateQuantumPathsTimerHandle,
            this,
            &AMatrixCodingNetwork::HandleManipulateQuantumPathsTick,
            FMath::Max(0.0f, ManipulateQuantumPathsInterval),
            true);
    }
}

void AMatrixCodingNetwork::InitializePaths()
{
    CodingPaths.Add(TEXT("PathA"), FCodingDataArray{});
    CodingPaths.Add(TEXT("PathB"), FCodingDataArray{});
    CodingPaths.Add(TEXT("PathC"), FCodingDataArray{});

    UE_LOG(LogTemp, Log, TEXT("Matrix Coding Network Initialized with Paths: PathA, PathB, PathC."));
}

void AMatrixCodingNetwork::AddCodingData(const FString& SourcePath, const FString& DestinationPath, float EnergyStrength, float CodeRate)
{
    if (!CodingPaths.Contains(SourcePath) || !CodingPaths.Contains(DestinationPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid source or destination path."));
        return;
    }

    const FCodingData Data(SourcePath, DestinationPath, EnergyStrength, CodeRate);
    CodingPaths.FindChecked(SourcePath).Items.Add(Data);
    ActiveCodings.Add(Data.ID, Data);

    UE_LOG(LogTemp, Log, TEXT("Added Coding Data - ID: %s, Source: %s, Destination: %s, Energy: %.2f, Rate: %.2f"), *Data.ID, *SourcePath, *DestinationPath, EnergyStrength, CodeRate);
}

void AMatrixCodingNetwork::AdaptCodingPaths()
{
    UE_LOG(LogTemp, Log, TEXT("Adapting coding paths to avoid collisions..."));

    for (TPair<FString, FCodingDataArray>& SourcePair : CodingPaths)
    {
        TArray<FCodingData>& List = SourcePair.Value.Items;
        for (int32 Index = 0; Index < List.Num(); /* increment inside */)
        {
            FCodingData& Data = List[Index];
            const FString NewPath = CalculateNewPath(Data);
            if (NewPath != Data.DestinationPath)
            {
                const FCodingData NewData(Data.SourcePath, NewPath, Data.EnergyStrength, Data.CodeRate);

                // Remove from current list and do not increment index
                List.RemoveAt(Index);

                // Insert into new destination path
                if (FCodingDataArray* DestList = CodingPaths.Find(NewPath))
                {
                    DestList->Items.Add(NewData);
                }
                else
                {
                    // Ensure path exists in case CalculateNewPath returns unexpected key
                    FCodingDataArray NewArr; NewArr.Items.Add(NewData);
                    CodingPaths.Add(NewPath, NewArr);
                }

                ActiveCodings.Add(NewData.ID, NewData);
                UE_LOG(LogTemp, Log, TEXT("Adapted Coding Data - ID: %s, New Destination Path: %s"), *NewData.ID, *NewPath);
            }
            else
            {
                ++Index;
            }
        }
    }
}

FString AMatrixCodingNetwork::CalculateNewPath(const FCodingData& Data) const
{
    if (Data.EnergyStrength > 50.f)
    {
        return TEXT("PathB");
    }
    else if (Data.CodeRate > 5.f)
    {
        return TEXT("PathC");
    }
    return TEXT("PathA");
}

void AMatrixCodingNetwork::ProcessCodingData()
{
    UE_LOG(LogTemp, Log, TEXT("Processing all active coding data..."));
    for (const TPair<FString, FCodingData>& Pair : ActiveCodings)
    {
        const FCodingData& Data = Pair.Value;
        UE_LOG(LogTemp, Log, TEXT("Processing Coding - ID: %s, Energy: %.2f, Rate: %.2f"), *Data.ID, Data.EnergyStrength, Data.CodeRate);
    }
}

void AMatrixCodingNetwork::ManipulateQuantumPaths()
{
    UE_LOG(LogTemp, Log, TEXT("Performing quantum path manipulation..."));
    AdaptCodingPaths();
}

// Timers
void AMatrixCodingNetwork::HandleAddCodingDataTick()
{
    AddCodingData(TEXT("PathA"), TEXT("PathC"), FMath::FRandRange(10.f, 100.f), FMath::FRandRange(1.f, 10.f));
}

void AMatrixCodingNetwork::HandleProcessCodingDataTick()
{
    ProcessCodingData();
}

void AMatrixCodingNetwork::HandleManipulateQuantumPathsTick()
{
    ManipulateQuantumPaths();
}
