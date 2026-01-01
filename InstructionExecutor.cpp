#include "InstructionExecutor.h"

#include "Containers/StringConv.h"
#include "Misc/DefaultValueHelper.h"

FString UInstructionExecutor::ExecuteInstructions(const TArray<FString>& Instructions, const FString& Input) const
{
    FString CurrentOutput = Input;

    for (const FString& Instruction : Instructions)
    {
        const FString Lower = Instruction.ToLower();

        if (Lower.Contains(TEXT("summarize")))
        {
            CurrentOutput = Summarize(CurrentOutput);
        }
        else if (Lower.Contains(TEXT("translate into spanish")))
        {
            CurrentOutput = Translate(CurrentOutput, TEXT("Spanish"));
        }
        else if (Registry.HasFunction(Instruction))
        {
            // Attempt to parse integer arguments from the instruction text
            TArray<int32> Args;
            ParseIntsFromString(Instruction, Args);

            FString Error;
            const TOptional<int32> Result = Registry.Invoke(Instruction, Args, &Error);
            if (Result.IsSet())
            {
                CurrentOutput = FString::FromInt(Result.GetValue());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Instruction '%s' failed: %s"), *Instruction, *Error);
            }
        }
    }

    return CurrentOutput;
}

FString UInstructionExecutor::Summarize(const FString& Text) const
{
    return FString::Printf(TEXT("(Summary of: %s)"), *Text);
}

FString UInstructionExecutor::Translate(const FString& Text, const FString& Language) const
{
    return FString::Printf(TEXT("(Translation of %s to %s)"), *Text, *Language);
}

void UInstructionExecutor::ParseIntsFromString(const FString& S, TArray<int32>& OutInts)
{
    OutInts.Reset();

    int32 i = 0;
    const int32 N = S.Len();
    while (i < N)
    {
        // Skip non-digit and non-sign characters
        while (i < N && !FChar::IsDigit(S[i]) && S[i] != '-') { ++i; }
        if (i >= N) break;

        const int32 Start = i;
        if (S[i] == '-') { ++i; }
        while (i < N && FChar::IsDigit(S[i])) { ++i; }

        const int32 End = i; // exclusive
        const FString NumStr = S.Mid(Start, End - Start);
        int32 Value = 0;
        if (FDefaultValueHelper::ParseInt(NumStr, Value))
        {
            OutInts.Add(Value);
        }
    }
}
