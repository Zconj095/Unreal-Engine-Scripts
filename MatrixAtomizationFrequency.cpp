#include "MatrixAtomizationFrequency.h"

#include "Misc/Guid.h"
#include "Engine/World.h"
#include "TimerManager.h"

AMatrixAtomizationFrequency::AMatrixAtomizationFrequency()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixAtomizationFrequency::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Atomization Frequency System Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddAtomTimerHandle,
            this,
            &AMatrixAtomizationFrequency::HandleAddAtomTick,
            FMath::Max(0.0f, AddAtomInterval),
            true);

        World->GetTimerManager().SetTimer(
            CalibrateFluxTimerHandle,
            this,
            &AMatrixAtomizationFrequency::HandleCalibrateFluxTick,
            FMath::Max(0.0f, CalibrateFluxInterval),
            true);

        World->GetTimerManager().SetTimer(
            GetInfoTimerHandle,
            this,
            &AMatrixAtomizationFrequency::HandleGetInfoTick,
            FMath::Max(0.0f, GetInfoInterval),
            true);
    }
}

FString AMatrixAtomizationFrequency::GenerateSerialCode() const
{
    return FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
}

float AMatrixAtomizationFrequency::CalculateFrequency(const FString& SerialCode) const
{
    // Simple frequency formula based on serial code hash
    const uint32 Hash = GetTypeHash(SerialCode);
    const float BaseFrequency = 1.0f;
    const float CalibratedFrequency = BaseFrequency + static_cast<float>(Hash % 100) / 10.0f;
    return FMath::Abs(CalibratedFrequency);
}

void AMatrixAtomizationFrequency::AddAtom()
{
    const FString SerialCode = GenerateSerialCode();
    const float Frequency = CalculateFrequency(SerialCode);

    FAtom Atom(SerialCode, Frequency);
    AtomNetwork.Add(SerialCode, Atom);
    LastAddedSerial = SerialCode;

    UE_LOG(LogTemp, Log, TEXT("Added Atom - Serial Code: %s, Frequency: %.3f Hz"), *SerialCode, Frequency);
}

void AMatrixAtomizationFrequency::GetAtomInfo(const FString& SerialCode) const
{
    if (const FAtom* Atom = AtomNetwork.Find(SerialCode))
    {
        UE_LOG(LogTemp, Log, TEXT("Atom Info - Serial Code: %s, Frequency: %.3f Hz"), *Atom->SerialCode, Atom->Frequency);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Atom with Serial Code '%s' not found."), *SerialCode);
    }
}

void AMatrixAtomizationFrequency::GetLastAtomInfo() const
{
    if (AtomNetwork.Num() > 0 && !LastAddedSerial.IsEmpty())
    {
        GetAtomInfo(LastAddedSerial);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No atoms in the network."));
    }
}

void AMatrixAtomizationFrequency::CalibrateFlux()
{
    UE_LOG(LogTemp, Log, TEXT("Calibrating spatial-temporal flux for all atoms..."));
    for (TPair<FString, FAtom>& Pair : AtomNetwork)
    {
        const float Delta = FMath::FRandRange(-0.1f, 0.1f);
        const float Recalibrated = Pair.Value.Frequency + Delta;
        UE_LOG(LogTemp, Log, TEXT("Recalibrated Atom - Serial Code: %s, New Frequency: %.3f Hz"), *Pair.Value.SerialCode, Recalibrated);
    }
}

// Timers
void AMatrixAtomizationFrequency::HandleAddAtomTick()
{
    AddAtom();
}

void AMatrixAtomizationFrequency::HandleCalibrateFluxTick()
{
    CalibrateFlux();
}

void AMatrixAtomizationFrequency::HandleGetInfoTick() const
{
    GetLastAtomInfo();
}

