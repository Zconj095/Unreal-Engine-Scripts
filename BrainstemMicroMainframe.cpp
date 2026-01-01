#include "BrainstemMicroMainframe.h"

#include "Algo/Reverse.h"
#include "Engine/World.h"
#include "TimerManager.h"

ABrainstemMicroMainframe::ABrainstemMicroMainframe()
{
    PrimaryActorTick.bCanEverTick = false; // We use timers instead of Tick
}

void ABrainstemMicroMainframe::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Initializing Brainstem Micro Analysis Mainframe..."));

    InitializeReservoirs();

    // Schedule periodic DNA analysis
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            DnaAnalysisTimerHandle,
            this,
            &ABrainstemMicroMainframe::HandleDnaAnalysisTick,
            FMath::Max(0.0f, DnaAnalysisIntervalSeconds),
            true
        );

        // Schedule periodic data transfer
        World->GetTimerManager().SetTimer(
            DataTransferTimerHandle,
            this,
            &ABrainstemMicroMainframe::HandleDataTransferTick,
            FMath::Max(0.0f, DataTransferIntervalSeconds),
            true
        );
    }
}

void ABrainstemMicroMainframe::AccelerateDNAAnalysis(const FString& DnaData)
{
    const FString AnalyzedData = AnalyzeDNA(DnaData);
    StoreInMemory(AnalyzedData);
}

void ABrainstemMicroMainframe::DisplayMemoryDatabank() const
{
    UE_LOG(LogTemp, Log, TEXT("Memory Databank Contents:"));
    for (const FString& Data : MemoryDatabank)
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Data);
    }
}

void ABrainstemMicroMainframe::InitializeReservoirs()
{
    const int32 Count = FMath::Max(1, ReservoirCount);
    InputReservoirs.SetNum(Count);
    OutputReservoirs.SetNum(Count);

    for (int32 Index = 0; Index < Count; ++Index)
    {
        const int32 HumanIndex = Index + 1;
        InputReservoirs[Index] = FDNAReservoir(FString::Printf(TEXT("Input DNA %d"), HumanIndex));
        OutputReservoirs[Index] = FDNAReservoir(FString::Printf(TEXT("Output DNA %d"), HumanIndex));
    }

    UE_LOG(LogTemp, Log, TEXT("Reservoirs initialized."));
}

void ABrainstemMicroMainframe::TransferDataToOutput()
{
    const int32 Count = InputReservoirs.Num();
    for (int32 Index = 0; Index < Count; ++Index)
    {
        const FString& Data = InputReservoirs[Index].DNASequence;
        OutputReservoirs[Index] = FDNAReservoir(Data);
        UE_LOG(LogTemp, Log, TEXT("Transferred data from Input Reservoir %d to Output Reservoir %d"), Index + 1, Index + 1);
    }
}

FString ABrainstemMicroMainframe::AnalyzeDNA(const FString& DnaData) const
{
    FString Reversed = DnaData;
    if (Reversed.Len() > 1)
    {
        TCHAR* DataPtr = Reversed.GetCharArray().GetData();
        Algo::Reverse(DataPtr, Reversed.Len());
    }
    return Reversed;
}

void ABrainstemMicroMainframe::StoreInMemory(const FString& Data)
{
    MemoryDatabank.Add(Data);
    UE_LOG(LogTemp, Log, TEXT("Data stored in Memory Databank: %s"), *Data);
}

void ABrainstemMicroMainframe::HandleDnaAnalysisTick()
{
    // Mirrors Unity Update() periodic call: AccelerateDNAAnalysis("AGCTTAGGCT")
    AccelerateDNAAnalysis(TEXT("AGCTTAGGCT"));
}

void ABrainstemMicroMainframe::HandleDataTransferTick()
{
    TransferDataToOutput();
}
