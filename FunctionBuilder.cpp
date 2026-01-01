#include "FunctionBuilder.h"

void FFunctionBuilder::AddFunctionFromInstruction(const FString& Instruction, FFunctionRegistry& Registry)
{
    const FString Lower = Instruction.ToLower();

    if (Lower.Contains(TEXT("adds two numbers")))
    {
        Registry.RegisterFunction(TEXT("add"), [](const TArray<int32>& Args, int32& OutResult, FString& OutError) -> bool
        {
            if (Args.Num() != 2)
            {
                OutError = TEXT("Add function expects 2 arguments.");
                return false;
            }
            OutResult = Args[0] + Args[1];
            return true;
        });
    }
    else if (Lower.Contains(TEXT("calculates factorial")))
    {
        Registry.RegisterFunction(TEXT("factorial"), [](const TArray<int32>& Args, int32& OutResult, FString& OutError) -> bool
        {
            if (Args.Num() != 1)
            {
                OutError = TEXT("Factorial function expects 1 argument.");
                return false;
            }
            const int32 N = Args[0];
            if (N < 0)
            {
                OutError = TEXT("Factorial expects non-negative integer.");
                return false;
            }
            int32 Result = 1;
            for (int32 i = 1; i <= N; ++i)
            {
                Result *= i;
            }
            OutResult = Result;
            return true;
        });
    }
}

