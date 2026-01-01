#include "LLMDataStoreComponent.h"

ULLMDataStoreComponent::ULLMDataStoreComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

FLLMScopeData& ULLMDataStoreComponent::ResolveScope(ELLMScope Scope)
{
    switch (Scope)
    {
    case ELLMScope::Private:  return PrivateData;
    case ELLMScope::Personal: return PersonalData;
    case ELLMScope::Local:    return LocalData;
    default:                  return PublicData;
    }
}

const FLLMScopeData& ULLMDataStoreComponent::ResolveScope(ELLMScope Scope) const
{
    switch (Scope)
    {
    case ELLMScope::Private:  return PrivateData;
    case ELLMScope::Personal: return PersonalData;
    case ELLMScope::Local:    return LocalData;
    default:                  return PublicData;
    }
}

FLLMScopeArrays& ULLMDataStoreComponent::ResolveScopeArrays(ELLMScope Scope)
{
    switch (Scope)
    {
    case ELLMScope::Private:  return PrivateArrays;
    case ELLMScope::Personal: return PersonalArrays;
    case ELLMScope::Local:    return LocalArrays;
    default:                  return PublicArrays;
    }
}

const FLLMScopeArrays& ULLMDataStoreComponent::ResolveScopeArrays(ELLMScope Scope) const
{
    switch (Scope)
    {
    case ELLMScope::Private:  return PrivateArrays;
    case ELLMScope::Personal: return PersonalArrays;
    case ELLMScope::Local:    return LocalArrays;
    default:                  return PublicArrays;
    }
}

// Vars
void ULLMDataStoreComponent::SetInt(ELLMScope Scope, const FString& Key, int32 Value)
{
    ResolveScope(Scope).IntVars.Add(Key, Value);
}

bool ULLMDataStoreComponent::GetInt(ELLMScope Scope, const FString& Key, int32& OutValue) const
{
    if (const int32* Ptr = ResolveScope(Scope).IntVars.Find(Key)) { OutValue = *Ptr; return true; }
    return false;
}

void ULLMDataStoreComponent::SetFloat(ELLMScope Scope, const FString& Key, float Value)
{
    ResolveScope(Scope).FloatVars.Add(Key, Value);
}

bool ULLMDataStoreComponent::GetFloat(ELLMScope Scope, const FString& Key, float& OutValue) const
{
    if (const float* Ptr = ResolveScope(Scope).FloatVars.Find(Key)) { OutValue = *Ptr; return true; }
    return false;
}

void ULLMDataStoreComponent::SetDouble(ELLMScope Scope, const FString& Key, double Value)
{
    ResolveScope(Scope).DoubleVars.Add(Key, Value);
}

bool ULLMDataStoreComponent::GetDouble(ELLMScope Scope, const FString& Key, double& OutValue) const
{
    if (const double* Ptr = ResolveScope(Scope).DoubleVars.Find(Key)) { OutValue = *Ptr; return true; }
    return false;
}

void ULLMDataStoreComponent::SetString(ELLMScope Scope, const FString& Key, const FString& Value)
{
    ResolveScope(Scope).StringVars.Add(Key, Value);
}

bool ULLMDataStoreComponent::GetString(ELLMScope Scope, const FString& Key, FString& OutValue) const
{
    if (const FString* Ptr = ResolveScope(Scope).StringVars.Find(Key)) { OutValue = *Ptr; return true; }
    return false;
}

// Arrays
void ULLMDataStoreComponent::SetIntArray(ELLMScope Scope, const FString& Key, const TArray<int32>& Values)
{
    FLLMIntArray Wrapper; Wrapper.Items = Values;
    ResolveScopeArrays(Scope).IntArrays.Add(Key, Wrapper);
}

bool ULLMDataStoreComponent::GetIntArray(ELLMScope Scope, const FString& Key, TArray<int32>& OutValues) const
{
    if (const FLLMIntArray* Ptr = ResolveScopeArrays(Scope).IntArrays.Find(Key)) { OutValues = Ptr->Items; return true; }
    return false;
}

void ULLMDataStoreComponent::SetFloatArray(ELLMScope Scope, const FString& Key, const TArray<float>& Values)
{
    FLLMFloatArray Wrapper; Wrapper.Items = Values;
    ResolveScopeArrays(Scope).FloatArrays.Add(Key, Wrapper);
}

bool ULLMDataStoreComponent::GetFloatArray(ELLMScope Scope, const FString& Key, TArray<float>& OutValues) const
{
    if (const FLLMFloatArray* Ptr = ResolveScopeArrays(Scope).FloatArrays.Find(Key)) { OutValues = Ptr->Items; return true; }
    return false;
}

void ULLMDataStoreComponent::SetDoubleArray(ELLMScope Scope, const FString& Key, const TArray<double>& Values)
{
    FLLMDoubleArray Wrapper; Wrapper.Items = Values;
    ResolveScopeArrays(Scope).DoubleArrays.Add(Key, Wrapper);
}

bool ULLMDataStoreComponent::GetDoubleArray(ELLMScope Scope, const FString& Key, TArray<double>& OutValues) const
{
    if (const FLLMDoubleArray* Ptr = ResolveScopeArrays(Scope).DoubleArrays.Find(Key)) { OutValues = Ptr->Items; return true; }
    return false;
}

void ULLMDataStoreComponent::SetStringArray(ELLMScope Scope, const FString& Key, const TArray<FString>& Values)
{
    FLLMStringArray Wrapper; Wrapper.Items = Values;
    ResolveScopeArrays(Scope).StringArrays.Add(Key, Wrapper);
}

bool ULLMDataStoreComponent::GetStringArray(ELLMScope Scope, const FString& Key, TArray<FString>& OutValues) const
{
    if (const FLLMStringArray* Ptr = ResolveScopeArrays(Scope).StringArrays.Find(Key)) { OutValues = Ptr->Items; return true; }
    return false;
}

// Tuples
void ULLMDataStoreComponent::SetTupleStrings(ELLMScope Scope, const FString& Key, const TArray<FString>& Items)
{
    FLLMStringArray Wrapper; Wrapper.Items = Items;
    ResolveScopeArrays(Scope).StringTuples.Add(Key, Wrapper);
}

bool ULLMDataStoreComponent::GetTupleStrings(ELLMScope Scope, const FString& Key, TArray<FString>& OutItems) const
{
    if (const FLLMStringArray* Ptr = ResolveScopeArrays(Scope).StringTuples.Find(Key)) { OutItems = Ptr->Items; return true; }
    return false;
}

// Params
void ULLMDataStoreComponent::SetParamNumber(ELLMScope Scope, const FString& Key, double Value)
{
    ResolveScope(Scope).NumericParams.Add(Key, Value);
}

bool ULLMDataStoreComponent::GetParamNumber(ELLMScope Scope, const FString& Key, double& OutValue) const
{
    if (const double* Ptr = ResolveScope(Scope).NumericParams.Find(Key)) { OutValue = *Ptr; return true; }
    return false;
}

void ULLMDataStoreComponent::SetParamString(ELLMScope Scope, const FString& Key, const FString& Value)
{
    ResolveScope(Scope).StringParams.Add(Key, Value);
}

bool ULLMDataStoreComponent::GetParamString(ELLMScope Scope, const FString& Key, FString& OutValue) const
{
    if (const FString* Ptr = ResolveScope(Scope).StringParams.Find(Key)) { OutValue = *Ptr; return true; }
    return false;
}

// Structs as JSON
void ULLMDataStoreComponent::SetStructJson(ELLMScope Scope, const FString& Key, const FString& Json)
{
    ResolveScope(Scope).StructJson.Add(Key, Json);
}

bool ULLMDataStoreComponent::GetStructJson(ELLMScope Scope, const FString& Key, FString& OutJson) const
{
    if (const FString* Ptr = ResolveScope(Scope).StructJson.Find(Key)) { OutJson = *Ptr; return true; }
    return false;
}
