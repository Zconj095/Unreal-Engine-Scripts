#include "MatrixAtomizerCode.h"

#include "Misc/Guid.h"
#include "Engine/World.h"
#include "TimerManager.h"

AMatrixAtomizerCode::AMatrixAtomizerCode()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixAtomizerCode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Atomizer Code Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            CreateCommandTimerHandle,
            this,
            &AMatrixAtomizerCode::HandleCreateCommandTick,
            FMath::Max(0.0f, CreateCommandInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveCommandTimerHandle,
            this,
            &AMatrixAtomizerCode::HandleRetrieveCommandTick,
            FMath::Max(0.0f, RetrieveCommandInterval),
            true);

        World->GetTimerManager().SetTimer(
            ManipulateFieldTimerHandle,
            this,
            &AMatrixAtomizerCode::HandleManipulateFieldTick,
            FMath::Max(0.0f, ManipulateFieldInterval),
            true);
    }
}

FString AMatrixAtomizerCode::GenerateSerialCode() const
{
    return FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
}

float AMatrixAtomizerCode::CalculateFrequency(const FString& SerialCode) const
{
    // Frequency in range [1, 10] Hz based on hash
    const uint32 Hash = GetTypeHash(SerialCode);
    const float Value = static_cast<float>((Hash % 100) + 10) / 10.0f;
    return FMath::Abs(Value);
}

FString AMatrixAtomizerCode::GeneratePixelData() const
{
    const int32 PixelValue = FMath::RandRange(0, 255); // 8-bit grayscale
    return FString::Printf(TEXT("PixelValue: %d"), PixelValue);
}

void AMatrixAtomizerCode::CreateAtomizerCommand()
{
    const FString SerialCode = GenerateSerialCode();
    const float Frequency = CalculateFrequency(SerialCode);
    const FString PixelData = GeneratePixelData();

    AtomCommands.Add(SerialCode, FAtomCommand(SerialCode, Frequency, PixelData));
    LastSerialCode = SerialCode;

    UE_LOG(LogTemp, Log, TEXT("Atomizer Command Created - Serial Code: %s, Frequency: %.3f Hz, Pixel Data: %s"), *SerialCode, Frequency, *PixelData);
}

void AMatrixAtomizerCode::GetAtomizerCommand(const FString& SerialCode) const
{
    if (const FAtomCommand* Cmd = AtomCommands.Find(SerialCode))
    {
        UE_LOG(LogTemp, Log, TEXT("Retrieved Atomizer Command - Serial Code: %s, Frequency: %.3f Hz, Pixel Data: %s"), *Cmd->SerialCode, Cmd->Frequency, *Cmd->DigitalPixelData);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Atomizer Command found with Serial Code: %s"), *SerialCode);
    }
}

void AMatrixAtomizerCode::ManipulateFieldFrequency(const FString& SerialCode) const
{
    if (const FAtomCommand* Cmd = AtomCommands.Find(SerialCode))
    {
        UE_LOG(LogTemp, Log, TEXT("Manipulating Field Frequency for Serial Code: %s at Frequency: %.3f Hz"), *SerialCode, Cmd->Frequency);
        UE_LOG(LogTemp, Log, TEXT("Using Digital Pixel Data: %s"), *Cmd->DigitalPixelData);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Atomizer Command found with Serial Code: %s"), *SerialCode);
    }
}

void AMatrixAtomizerCode::RetrieveLastCommand() const
{
    if (AtomCommands.Num() > 0 && !LastSerialCode.IsEmpty())
    {
        GetAtomizerCommand(LastSerialCode);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No atomizer commands available."));
    }
}

void AMatrixAtomizerCode::ManipulateLastCommandField() const
{
    if (AtomCommands.Num() > 0 && !LastSerialCode.IsEmpty())
    {
        ManipulateFieldFrequency(LastSerialCode);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No atomizer commands available."));
    }
}

// Timers
void AMatrixAtomizerCode::HandleCreateCommandTick()
{
    CreateAtomizerCommand();
}

void AMatrixAtomizerCode::HandleRetrieveCommandTick() const
{
    RetrieveLastCommand();
}

void AMatrixAtomizerCode::HandleManipulateFieldTick() const
{
    ManipulateLastCommandField();
}

