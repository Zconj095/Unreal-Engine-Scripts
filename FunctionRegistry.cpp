#include "FunctionRegistry.h"

void FFunctionRegistry::RegisterFunction(const FString& Name, TFunction<bool(const TArray<int32>&, int32&, FString&)> Func)
{
    FRegisteredFunction RF; RF.Func = MoveTemp(Func);
    Registry.Add(Name, MoveTemp(RF));
}

bool FFunctionRegistry::HasFunction(const FString& Name) const
{
    return Registry.Contains(Name);
}

TOptional<int32> FFunctionRegistry::Invoke(const FString& Name, const TArray<int32>& Args, FString* OutError) const
{
    const FRegisteredFunction* Found = Registry.Find(Name);
    if (!Found)
    {
        if (OutError) { *OutError = FString::Printf(TEXT("Function '%s' not found."), *Name); }
        return TOptional<int32>();
    }

    int32 Result = 0;
    FString Err;
    const bool bOk = Found->Func(Args, Result, Err);
    if (!bOk)
    {
        if (OutError) { *OutError = Err; }
        return TOptional<int32>();
    }
    return TOptional<int32>(Result);
}

TArray<FString> FFunctionRegistry::GetFunctionNames() const
{
    TArray<FString> Names;
    Registry.GetKeys(Names);
    Names.Sort();
    return Names;
}

