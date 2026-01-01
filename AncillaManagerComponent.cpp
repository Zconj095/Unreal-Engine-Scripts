// Copyright Epic Games, Inc.

#include "AncillaManagerComponent.h"

UAncillaManagerComponent::UAncillaManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAncillaManagerComponent::InitializeRegisters()
{
    PrimaryRegisters.Reset();
    AncillaRegisters.Reset();

    const int32 PrimaryCount = FMath::Max(0, PrimaryQubitCount);
    const int32 AncillaCount = FMath::Max(0, AncillaQubitCount);

    PrimaryRegisters.Reserve(PrimaryCount);
    for (int32 Index = 0; Index < PrimaryCount; ++Index)
    {
        PrimaryRegisters.Add(FString::Printf(TEXT("q%d"), Index));
    }

    AncillaRegisters.Reserve(AncillaCount);
    for (int32 Index = 0; Index < AncillaCount; ++Index)
    {
        AncillaRegisters.Add(FString::Printf(TEXT("a%d"), Index));
    }

    UE_LOG(LogTemp, Log, TEXT("Primary Registers: %s"), *FString::Join(PrimaryRegisters, TEXT(", ")));
    UE_LOG(LogTemp, Log, TEXT("Ancilla Registers: %s"), *FString::Join(AncillaRegisters, TEXT(", ")));
}

FString UAncillaManagerComponent::GetAncilla(int32 Index) const
{
    if (!AncillaRegisters.IsValidIndex(Index))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid ancilla index %d."), Index);
        return FString();
    }

    return AncillaRegisters[Index];
}

void UAncillaManagerComponent::AddAncilla()
{
    const FString NewAncilla = FString::Printf(TEXT("a%d"), AncillaRegisters.Num());
    AncillaRegisters.Add(NewAncilla);
    UE_LOG(LogTemp, Log, TEXT("Added Ancilla: %s"), *NewAncilla);
}

void UAncillaManagerComponent::RemoveAncilla(int32 Index)
{
    if (!AncillaRegisters.IsValidIndex(Index))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid ancilla index %d."), Index);
        return;
    }

    const FString Removed = AncillaRegisters[Index];
    AncillaRegisters.RemoveAt(Index);
    UE_LOG(LogTemp, Log, TEXT("Removed Ancilla: %s"), *Removed);
}
