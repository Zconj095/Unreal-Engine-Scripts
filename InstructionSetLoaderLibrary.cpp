#include "InstructionSetLoaderLibrary.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UInstructionSetLoaderLibrary::LoadInstructionSet(const FString& FilePath, FInstructionSet& OutSet, bool& bSuccess, FString& OutError)
{
    bSuccess = false;
    OutError.Reset();
    OutSet = FInstructionSet();

    if (FilePath.IsEmpty())
    {
        OutError = TEXT("FilePath is empty");
        return;
    }

    TArray<FString> Lines;
    if (!FFileHelper::LoadFileToStringArray(Lines, *FilePath))
    {
        OutError = FString::Printf(TEXT("Instruction set file not found or unreadable: %s"), *FilePath);
        return;
    }

    FString Name;
    TArray<FString> Instructions;

    const FString NamePrefix = TEXT("InstructionSet Name:");
    const FString InstructionPrefix = TEXT("Instruction:");

    for (FString Line : Lines)
    {
        Line.TrimStartAndEndInline();
        if (Line.IsEmpty()) continue;

        if (Line.StartsWith(NamePrefix, ESearchCase::CaseSensitive))
        {
            FString Value = Line.Mid(NamePrefix.Len());
            Value.TrimStartAndEndInline();
            Name = MoveTemp(Value);
        }
        else if (Line.StartsWith(InstructionPrefix, ESearchCase::CaseSensitive))
        {
            FString Value = Line.Mid(InstructionPrefix.Len());
            Value.TrimStartAndEndInline();
            if (!Value.IsEmpty())
            {
                Instructions.Add(MoveTemp(Value));
            }
        }
        // Other lines (e.g., Instruction Count) are ignored.
    }

    OutSet.Name = MoveTemp(Name);
    OutSet.Instructions = MoveTemp(Instructions);
    bSuccess = true;
}

