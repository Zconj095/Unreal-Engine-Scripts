#include "MatrixEntanglement.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixEntanglement::AMatrixEntanglement()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixEntanglement::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Entanglement Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            CreateServerTimerHandle,
            this,
            &AMatrixEntanglement::HandleCreateServerTick,
            FMath::Max(0.0f, CreateServerInterval),
            true);

        World->GetTimerManager().SetTimer(
            AddWordTimerHandle,
            this,
            &AMatrixEntanglement::HandleAddWordTick,
            FMath::Max(0.0f, AddWordInterval),
            true);

        World->GetTimerManager().SetTimer(
            EntangleWordsTimerHandle,
            this,
            &AMatrixEntanglement::HandleEntangleWordsTick,
            FMath::Max(0.0f, EntangleWordsInterval),
            true);

        World->GetTimerManager().SetTimer(
            CreateDomainTimerHandle,
            this,
            &AMatrixEntanglement::HandleCreateDomainTick,
            FMath::Max(0.0f, CreateDomainInterval),
            true);

        World->GetTimerManager().SetTimer(
            AddServerToDomainTimerHandle,
            this,
            &AMatrixEntanglement::HandleAddServerToDomainTick,
            FMath::Max(0.0f, AddServerToDomainInterval),
            true);
    }
}

int32 AMatrixEntanglement::FindServerIndexById(const FString& InServerID) const
{
    for (int32 Index = 0; Index < Servers.Num(); ++Index)
    {
        if (Servers[Index].ServerID == InServerID)
        {
            return Index;
        }
    }
    return INDEX_NONE;
}

void AMatrixEntanglement::CreateServer()
{
    const FString NewID = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
    FMatrixServerData Server(NewID);
    Servers.Add(MoveTemp(Server));
    UE_LOG(LogTemp, Log, TEXT("Server Created - ID: %s"), *NewID);
}

void AMatrixEntanglement::AddWordToServer(const FString& Word, const FString& ServerID)
{
    const int32 ServerIdx = FindServerIndexById(ServerID);
    if (ServerIdx != INDEX_NONE)
    {
        Servers[ServerIdx].AddWordNode(FWordNode(Word));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Server with ID '%s' not found."), *ServerID);
    }
}

void AMatrixEntanglement::EntangleWords(const FString& Word, const FString& ProtonWord, const FString& ElectronWord, const FString& ServerID)
{
    const int32 ServerIdx = FindServerIndexById(ServerID);
    if (ServerIdx != INDEX_NONE)
    {
        FMatrixServerData& Server = Servers[ServerIdx];
        FWordNode* WordNode = nullptr;
        FWordNode* ProtonNode = nullptr;
        FWordNode* ElectronNode = nullptr;

        for (FWordNode& Node : Server.EntangledWords)
        {
            if (Node.Word == Word) { WordNode = &Node; }
            if (Node.Word == ProtonWord) { ProtonNode = &Node; }
            if (Node.Word == ElectronWord) { ElectronNode = &Node; }
        }

        if (WordNode && ProtonNode && ElectronNode)
        {
            WordNode->Entangle(ProtonNode->Word, ElectronNode->Word);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("One or more words not found in the server."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Server with ID '%s' not found."), *ServerID);
    }
}

void AMatrixEntanglement::CreateDomain(const FString& DomainName)
{
    if (!Domains.Contains(DomainName))
    {
        Domains.Add(DomainName, FStringArrayContainer{});
        UE_LOG(LogTemp, Log, TEXT("Domain Created - Name: %s"), *DomainName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Domain '%s' already exists."), *DomainName);
    }
}

void AMatrixEntanglement::AddServerToDomain(const FString& DomainName, const FString& ServerID)
{
    if (FStringArrayContainer* ServerList = Domains.Find(DomainName))
    {
        const int32 ServerIdx = FindServerIndexById(ServerID);
        if (ServerIdx != INDEX_NONE)
        {
            ServerList->Items.Add(ServerID);
            UE_LOG(LogTemp, Log, TEXT("Server '%s' added to Domain '%s'."), *ServerID, *DomainName);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Server with ID '%s' not found."), *ServerID);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Domain '%s' does not exist."), *DomainName);
    }
}

// Timers
void AMatrixEntanglement::HandleCreateServerTick()
{
    CreateServer();
}

void AMatrixEntanglement::HandleAddWordTick()
{
    if (Servers.Num() > 0)
    {
        const FString& LastServerID = Servers.Last().ServerID;
        const int32 Rand = FMath::RandRange(1, 99);
        AddWordToServer(FString::Printf(TEXT("Word_%d"), Rand), LastServerID);
    }
}

void AMatrixEntanglement::HandleEntangleWordsTick()
{
    if (Servers.Num() > 0)
    {
        FMatrixServerData& LastServer = Servers.Last();
        if (LastServer.EntangledWords.Num() >= 3)
        {
            const FString& W = LastServer.EntangledWords[0].Word;
            const FString& P = LastServer.EntangledWords[1].Word;
            const FString& E = LastServer.EntangledWords[2].Word;
            EntangleWords(W, P, E, LastServer.ServerID);
        }
    }
}

void AMatrixEntanglement::HandleCreateDomainTick()
{
    const FString Name = FString::Printf(TEXT("Domain_%d"), Domains.Num() + 1);
    CreateDomain(Name);
}

void AMatrixEntanglement::HandleAddServerToDomainTick()
{
    if (Domains.Num() > 0 && Servers.Num() > 0)
    {
        const FString DomainName = FString::Printf(TEXT("Domain_%d"), Domains.Num());
        AddServerToDomain(DomainName, Servers.Last().ServerID);
    }
}
