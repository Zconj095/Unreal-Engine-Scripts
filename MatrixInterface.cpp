#include "MatrixInterface.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixInterface::AMatrixInterface()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixInterface::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Interface Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            CreateNetworkTimerHandle,
            this,
            &AMatrixInterface::HandleCreateNetworkTick,
            FMath::Max(0.0f, CreateNetworkInterval),
            true);

        World->GetTimerManager().SetTimer(
            AddJointTimerHandle,
            this,
            &AMatrixInterface::HandleAddJointTick,
            FMath::Max(0.0f, AddJointInterval),
            true);

        World->GetTimerManager().SetTimer(
            BindNetworkTimerHandle,
            this,
            &AMatrixInterface::HandleBindNetworkTick,
            FMath::Max(0.0f, BindNetworkInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveNetworksTimerHandle,
            this,
            &AMatrixInterface::HandleRetrieveNetworksTick,
            FMath::Max(0.0f, RetrieveNetworksInterval),
            true);
    }
}

void AMatrixInterface::CreateNetwork()
{
    const FString NewID = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
    Networks.Add(FStitchedNetwork(NewID));
    UE_LOG(LogTemp, Log, TEXT("Network Created - ID: %s"), *NewID);
}

void AMatrixInterface::AddJointToNetwork(const FString& Path, const FString& NetworkID)
{
    for (FStitchedNetwork& Net : Networks)
    {
        if (Net.NetworkID == NetworkID)
        {
            Net.AddJoint(FMatrixJoint(Path));
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Network with ID '%s' not found."), *NetworkID);
}

void AMatrixInterface::BindNetwork(const FString& NetworkID)
{
    for (FStitchedNetwork& Net : Networks)
    {
        if (Net.NetworkID == NetworkID)
        {
            Net.BindAllJoints();
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Network with ID '%s' not found."), *NetworkID);
}

void AMatrixInterface::RetrieveNetworks() const
{
    UE_LOG(LogTemp, Log, TEXT("Retrieving all networks..."));
    for (const FStitchedNetwork& Net : Networks)
    {
        UE_LOG(LogTemp, Log, TEXT("Network - ID: %s, Total Joints: %d"), *Net.NetworkID, Net.Joints.Num());
        for (const FMatrixJoint& Joint : Net.Joints)
        {
            UE_LOG(LogTemp, Log, TEXT("  Joint - ID: %s, Path: %s, Entangled: %s"), *Joint.JointID, *Joint.Path, Joint.bIsEntangled ? TEXT("true") : TEXT("false"));
        }
    }
}

// Timers
void AMatrixInterface::HandleCreateNetworkTick()
{
    CreateNetwork();
}

void AMatrixInterface::HandleAddJointTick()
{
    if (Networks.Num() > 0)
    {
        const FString& NetID = Networks.Last().NetworkID;
        const int32 Rand = FMath::RandRange(1, 99);
        AddJointToNetwork(FString::Printf(TEXT("Path_%d"), Rand), NetID);
    }
}

void AMatrixInterface::HandleBindNetworkTick()
{
    if (Networks.Num() > 0)
    {
        BindNetwork(Networks.Last().NetworkID);
    }
}

void AMatrixInterface::HandleRetrieveNetworksTick() const
{
    RetrieveNetworks();
}

