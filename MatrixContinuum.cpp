#include "MatrixContinuum.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixContinuum::AMatrixContinuum()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixContinuum::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Continuum Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddNodeTimerHandle,
            this,
            &AMatrixContinuum::HandleAddNodeTick,
            FMath::Max(0.0f, AddNodeInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveNodesTimerHandle,
            this,
            &AMatrixContinuum::HandleRetrieveNodesTick,
            FMath::Max(0.0f, RetrieveNodesInterval),
            true);

        World->GetTimerManager().SetTimer(
            SimulateFeedTimerHandle,
            this,
            &AMatrixContinuum::HandleSimulateFeedTick,
            FMath::Max(0.0f, SimulateFeedInterval),
            true);
    }
}

bool AMatrixContinuum::IsValidMemoryType(const FString& MemoryType)
{
    return MemoryType.Equals(TEXT("Past"), ESearchCase::IgnoreCase)
        || MemoryType.Equals(TEXT("Present"), ESearchCase::IgnoreCase)
        || MemoryType.Equals(TEXT("Future"), ESearchCase::IgnoreCase);
}

void AMatrixContinuum::AddNode(const FString& Domain, const FString& MemoryType, const FString& Data)
{
    if (!IsValidMemoryType(MemoryType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid memory type. Must be 'Past', 'Present', or 'Future'."));
        return;
    }

    const FContinuumNode Node(Domain, MemoryType, Data);
    ContinuumGrid.Add(Node);

    UE_LOG(LogTemp, Log, TEXT("Node Added - ID: %s, Domain: %s, MemoryType: %s, Data: %s"), *Node.ID, *Domain, *MemoryType, *Data);
}

void AMatrixContinuum::RetrieveNodes(const FString& MemoryType) const
{
    if (!IsValidMemoryType(MemoryType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid memory type. Must be 'Past', 'Present', or 'Future'."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Retrieving nodes with MemoryType: %s"), *MemoryType);
    for (const FContinuumNode& Node : ContinuumGrid)
    {
        if (Node.MemoryType.Equals(MemoryType, ESearchCase::IgnoreCase))
        {
            UE_LOG(LogTemp, Log, TEXT("Node - ID: %s, Domain: %s, Data: %s"), *Node.ID, *Node.Domain, *Node.Data);
        }
    }
}

void AMatrixContinuum::SimulateSpatialFeed() const
{
    UE_LOG(LogTemp, Log, TEXT("Simulating spatial feed of the continuum..."));
    for (const FContinuumNode& Node : ContinuumGrid)
    {
        UE_LOG(LogTemp, Log, TEXT("Streaming Node - ID: %s, Domain: %s, MemoryType: %s, Data: %s"), *Node.ID, *Node.Domain, *Node.MemoryType, *Node.Data);
    }
}

// Timers
void AMatrixContinuum::HandleAddNodeTick()
{
    const float R = FMath::FRand();
    const FString MemoryType = (R > 0.66f) ? TEXT("Past") : ((R > 0.33f) ? TEXT("Present") : TEXT("Future"));
    AddNode(TEXT("Automated Domain"), MemoryType, FString::Printf(TEXT("Sample data for %s."), *MemoryType));
}

void AMatrixContinuum::HandleRetrieveNodesTick() const
{
    const bool bPast = FMath::FRand() > 0.5f;
    RetrieveNodes(bPast ? TEXT("Past") : TEXT("Future"));
}

void AMatrixContinuum::HandleSimulateFeedTick() const
{
    SimulateSpatialFeed();
}

