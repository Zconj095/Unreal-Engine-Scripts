#include "ContextManager2.h"

void UContextManager2::SetContext(const FString& Key, const FContextValue& Value)
{
    Context.Add(Key, Value);
}

bool UContextManager2::GetContext(const FString& Key, FContextValue& OutValue) const
{
    if (const FContextValue* Found = Context.Find(Key))
    {
        OutValue = *Found;
        return true;
    }
    return false;
}

void UContextManager2::SetString(const FString& Key, const FString& Value)
{
    FContextValue V; V.Type = EContextValueType::String; V.StringValue = Value; Context.Add(Key, V);
}

void UContextManager2::SetFloat(const FString& Key, float Value)
{
    FContextValue V; V.Type = EContextValueType::Float; V.FloatValue = Value; Context.Add(Key, V);
}

void UContextManager2::SetInt(const FString& Key, int32 Value)
{
    FContextValue V; V.Type = EContextValueType::Int; V.IntValue = Value; Context.Add(Key, V);
}

void UContextManager2::SetBool(const FString& Key, bool Value)
{
    FContextValue V; V.Type = EContextValueType::Bool; V.BoolValue = Value; Context.Add(Key, V);
}

void UContextManager2::SetObject(const FString& Key, UObject* Value)
{
    FContextValue V; V.Type = EContextValueType::Object; V.ObjectValue = Value; Context.Add(Key, V);
}

bool UContextManager2::GetString(const FString& Key, FString& OutValue) const
{
    if (const FContextValue* Found = Context.Find(Key))
    {
        if (Found->Type == EContextValueType::String)
        {
            OutValue = Found->StringValue; return true;
        }
    }
    return false;
}

bool UContextManager2::GetFloat(const FString& Key, float& OutValue) const
{
    if (const FContextValue* Found = Context.Find(Key))
    {
        if (Found->Type == EContextValueType::Float)
        {
            OutValue = Found->FloatValue; return true;
        }
    }
    return false;
}

bool UContextManager2::GetInt(const FString& Key, int32& OutValue) const
{
    if (const FContextValue* Found = Context.Find(Key))
    {
        if (Found->Type == EContextValueType::Int)
        {
            OutValue = Found->IntValue; return true;
        }
    }
    return false;
}

bool UContextManager2::GetBool(const FString& Key, bool& OutValue) const
{
    if (const FContextValue* Found = Context.Find(Key))
    {
        if (Found->Type == EContextValueType::Bool)
        {
            OutValue = Found->BoolValue; return true;
        }
    }
    return false;
}

bool UContextManager2::GetObject(const FString& Key, UObject*& OutValue) const
{
    if (const FContextValue* Found = Context.Find(Key))
    {
        if (Found->Type == EContextValueType::Object)
        {
            OutValue = Found->ObjectValue; return true;
        }
    }
    return false;
}

