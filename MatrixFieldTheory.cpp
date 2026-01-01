#include "MatrixFieldTheory.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixFieldTheory::AMatrixFieldTheory()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixFieldTheory::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Field Theory System Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            GenerateFieldTimerHandle,
            this,
            &AMatrixFieldTheory::HandleGenerateFieldTick,
            FMath::Max(0.0f, GenerateFieldInterval),
            true);

        World->GetTimerManager().SetTimer(
            StitchFieldsTimerHandle,
            this,
            &AMatrixFieldTheory::HandleStitchFieldsTick,
            FMath::Max(0.0f, StitchFieldsInterval),
            true);

        World->GetTimerManager().SetTimer(
            ConvertFieldEnergyTimerHandle,
            this,
            &AMatrixFieldTheory::HandleConvertFieldEnergyTick,
            FMath::Max(0.0f, ConvertFieldEnergyInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveFieldsTimerHandle,
            this,
            &AMatrixFieldTheory::HandleRetrieveFieldsTick,
            FMath::Max(0.0f, RetrieveFieldsInterval),
            true);
    }
}

void AMatrixFieldTheory::GenerateField(const FString& Definition, float Frequency)
{
    FMatrixField NewField(Definition, Frequency);
    Fields.Add(NewField);
    UE_LOG(LogTemp, Log, TEXT("Field Generated - ID: %s, Definition: %s, Frequency: %.3f Hz"), *NewField.FieldID, *Definition, Frequency);

    // Check for paradoxes with existing fields
    for (FMatrixField& Field : Fields)
    {
        if (Field.FieldID != NewField.FieldID)
        {
            // Compare with copy in array, but we just added a value copy; handle with indices
        }
    }

    // Since we stored by value, re-iterate and set paradox flags using indices
    for (int32 i = 0; i < Fields.Num(); ++i)
    {
        for (int32 j = i + 1; j < Fields.Num(); ++j)
        {
            Fields[i].CheckForParadox(Fields[j]);
        }
    }
}

void AMatrixFieldTheory::StitchFields(const FString& FieldID1, const FString& FieldID2, float TimeGap)
{
    FMatrixField* Field1 = nullptr;
    FMatrixField* Field2 = nullptr;

    for (FMatrixField& F : Fields)
    {
        if (F.FieldID == FieldID1) { Field1 = &F; }
        if (F.FieldID == FieldID2) { Field2 = &F; }
    }

    if (Field1 && Field2)
    {
        if (Field1->bHasParadox || Field2->bHasParadox)
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot stitch fields with paradoxes."));
            return;
        }

        UE_LOG(LogTemp, Log, TEXT("Fields '%s' and '%s' stitched with a time gap of %.3f seconds."), *FieldID1, *FieldID2, TimeGap);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("One or both fields not found for stitching."));
    }
}

void AMatrixFieldTheory::ConvertFieldEnergy()
{
    UE_LOG(LogTemp, Log, TEXT("Converting field energy..."));
    for (const FMatrixField& Field : Fields)
    {
        if (!Field.bHasParadox)
        {
            const float EnergyOutput = Field.Frequency * 10.f; // Example energy conversion formula
            UE_LOG(LogTemp, Log, TEXT("Field '%s' converted to energy output: %.3f units."), *Field.FieldID, EnergyOutput);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Field '%s' has a paradox and cannot convert energy."), *Field.FieldID);
        }
    }
}

void AMatrixFieldTheory::RetrieveFields() const
{
    UE_LOG(LogTemp, Log, TEXT("Retrieving all fields..."));
    for (const FMatrixField& Field : Fields)
    {
        UE_LOG(LogTemp, Log, TEXT("Field - ID: %s, Definition: %s, Frequency: %.3f Hz, Paradox: %s"), *Field.FieldID, *Field.Definition, Field.Frequency, Field.bHasParadox ? TEXT("true") : TEXT("false"));
    }
}

// Timers
void AMatrixFieldTheory::HandleGenerateFieldTick()
{
    const FString Def = FString::Printf(TEXT("Definition_%d"), Fields.Num() + 1);
    const float Freq = FMath::FRandRange(1.f, 100.f);
    GenerateField(Def, Freq);
}

void AMatrixFieldTheory::HandleStitchFieldsTick()
{
    if (Fields.Num() >= 2)
    {
        StitchFields(Fields[0].FieldID, Fields[1].FieldID, FMath::FRandRange(0.1f, 5.f));
    }
}

void AMatrixFieldTheory::HandleConvertFieldEnergyTick()
{
    ConvertFieldEnergy();
}

void AMatrixFieldTheory::HandleRetrieveFieldsTick() const
{
    RetrieveFields();
}

