#include "DatabaseRetrievalLibrary.h"

#include "UObject/UnrealType.h"

static const FProperty* FindFieldChecked(const UScriptStruct* RowStruct, const FName FieldName)
{
    if (!RowStruct)
    {
        return nullptr;
    }
    return RowStruct->FindPropertyByName(FieldName);
}

bool UDatabaseRetrievalLibrary::EvaluateNumeric(double LHS, ENumericCompareOp Op, double RHS)
{
    switch (Op)
    {
    case ENumericCompareOp::Greater:      return LHS >  RHS;
    case ENumericCompareOp::GreaterEqual: return LHS >= RHS;
    case ENumericCompareOp::Less:         return LHS <  RHS;
    case ENumericCompareOp::LessEqual:    return LHS <= RHS;
    case ENumericCompareOp::Equal:        return LHS == RHS;
    case ENumericCompareOp::NotEqual:     return LHS != RHS;
    default: return false;
    }
}

bool UDatabaseRetrievalLibrary::EvaluateString(const FString& LHS, EStringCompareOp Op, const FString& RHS, bool bCaseSensitive)
{
    if (!bCaseSensitive)
    {
        if (Op == EStringCompareOp::Equal)    return LHS.Equals(RHS, ESearchCase::IgnoreCase);
        if (Op == EStringCompareOp::NotEqual) return !LHS.Equals(RHS, ESearchCase::IgnoreCase);
    }
    else
    {
        if (Op == EStringCompareOp::Equal)    return LHS.Equals(RHS, ESearchCase::CaseSensitive);
        if (Op == EStringCompareOp::NotEqual) return !LHS.Equals(RHS, ESearchCase::CaseSensitive);
    }
    return false;
}

bool UDatabaseRetrievalLibrary::TryGetNumericFieldValue(const UScriptStruct* RowStruct, const FProperty* Property, const void* RowData, double& OutValue)
{
    if (!Property || !RowData)
    {
        return false;
    }

    if (const FNumericProperty* NumProp = CastField<const FNumericProperty>(Property))
    {
        const void* ValuePtr = NumProp->ContainerPtrToValuePtr<void>(RowData);
        if (NumProp->IsFloatingPoint())
        {
            OutValue = NumProp->GetFloatingPointPropertyValue(ValuePtr);
            return true;
        }
        if (NumProp->IsInteger())
        {
            OutValue = (double)NumProp->GetSignedIntPropertyValue(ValuePtr);
            return true;
        }
    }
    if (const FBoolProperty* BoolProp = CastField<const FBoolProperty>(Property))
    {
        const void* ValuePtr = BoolProp->ContainerPtrToValuePtr<void>(RowData);
        OutValue = BoolProp->GetPropertyValue(ValuePtr) ? 1.0 : 0.0;
        return true;
    }
    return false;
}

bool UDatabaseRetrievalLibrary::TryGetStringFieldValue(const UScriptStruct* RowStruct, const FProperty* Property, const void* RowData, FString& OutValue)
{
    if (!Property || !RowData)
    {
        return false;
    }
    if (const FStrProperty* StrProp = CastField<const FStrProperty>(Property))
    {
        const void* ValuePtr = StrProp->ContainerPtrToValuePtr<void>(RowData);
        OutValue = StrProp->GetPropertyValue(ValuePtr);
        return true;
    }
    if (const FNameProperty* NameProp = CastField<const FNameProperty>(Property))
    {
        const void* ValuePtr = NameProp->ContainerPtrToValuePtr<void>(RowData);
        OutValue = NameProp->GetPropertyValue(ValuePtr).ToString();
        return true;
    }
    if (const FTextProperty* TextProp = CastField<const FTextProperty>(Property))
    {
        const void* ValuePtr = TextProp->ContainerPtrToValuePtr<void>(RowData);
        OutValue = TextProp->GetPropertyValue(ValuePtr).ToString();
        return true;
    }
    // Fallback: numeric to string
    double Num = 0.0;
    if (TryGetNumericFieldValue(RowStruct, Property, RowData, Num))
    {
        OutValue = FString::SanitizeFloat((float)Num);
        return true;
    }
    return false;
}

bool UDatabaseRetrievalLibrary::TryGetFieldAsString(const UScriptStruct* RowStruct, const FProperty* Property, const void* RowData, FString& OutValue)
{
    return TryGetStringFieldValue(RowStruct, Property, RowData, OutValue);
}

TArray<FName> UDatabaseRetrievalLibrary::RetrieveRowsByNumericCondition(UDataTable* Table, FName FieldName, ENumericCompareOp Op, double CompareValue)
{
    TArray<FName> Matches;
    if (!Table)
    {
        UE_LOG(LogTemp, Error, TEXT("[DatabaseRetrieval] Table is null"));
        return Matches;
    }
    const UScriptStruct* RowStruct = Table->GetRowStruct();
    const FProperty* Field = FindFieldChecked(RowStruct, FieldName);
    if (!Field)
    {
        UE_LOG(LogTemp, Error, TEXT("[DatabaseRetrieval] Field %s not found in row struct %s"), *FieldName.ToString(), *GetNameSafe(RowStruct));
        return Matches;
    }

    for (const TPair<FName, uint8*>& Pair : Table->GetRowMap())
    {
        const void* RowData = Pair.Value;
        double Value = 0.0;
        if (TryGetNumericFieldValue(RowStruct, Field, RowData, Value) && EvaluateNumeric(Value, Op, CompareValue))
        {
            Matches.Add(Pair.Key);
        }
    }
    return Matches;
}

TArray<FName> UDatabaseRetrievalLibrary::RetrieveRowsByStringCondition(UDataTable* Table, FName FieldName, EStringCompareOp Op, const FString& CompareValue, bool bCaseSensitive)
{
    TArray<FName> Matches;
    if (!Table)
    {
        UE_LOG(LogTemp, Error, TEXT("[DatabaseRetrieval] Table is null"));
        return Matches;
    }
    const UScriptStruct* RowStruct = Table->GetRowStruct();
    const FProperty* Field = FindFieldChecked(RowStruct, FieldName);
    if (!Field)
    {
        UE_LOG(LogTemp, Error, TEXT("[DatabaseRetrieval] Field %s not found in row struct %s"), *FieldName.ToString(), *GetNameSafe(RowStruct));
        return Matches;
    }

    for (const TPair<FName, uint8*>& Pair : Table->GetRowMap())
    {
        const void* RowData = Pair.Value;
        FString ValueStr;
        if (TryGetStringFieldValue(RowStruct, Field, RowData, ValueStr) && EvaluateString(ValueStr, Op, CompareValue, bCaseSensitive))
        {
            Matches.Add(Pair.Key);
        }
    }
    return Matches;
}

TArray<FStringArray> UDatabaseRetrievalLibrary::RetrieveColumnsByNumericCondition(UDataTable* Table, const TArray<FName>& Columns, FName FilterField, ENumericCompareOp Op, double CompareValue)
{
    TArray<FStringArray> Results;
    if (!Table)
    {
        UE_LOG(LogTemp, Error, TEXT("[DatabaseRetrieval] Table is null"));
        return Results;
    }
    const UScriptStruct* RowStruct = Table->GetRowStruct();
    const FProperty* FilterProp = FindFieldChecked(RowStruct, FilterField);
    if (!FilterProp)
    {
        UE_LOG(LogTemp, Error, TEXT("[DatabaseRetrieval] Filter field %s not found in row struct %s"), *FilterField.ToString(), *GetNameSafe(RowStruct));
        return Results;
    }

    // Cache column properties
    TArray<const FProperty*> ColumnProps; ColumnProps.Reserve(Columns.Num());
    for (const FName& Col : Columns)
    {
        ColumnProps.Add(FindFieldChecked(RowStruct, Col));
    }

    for (const TPair<FName, uint8*>& Pair : Table->GetRowMap())
    {
        const void* RowData = Pair.Value;
        double FilterValue = 0.0;
        if (TryGetNumericFieldValue(RowStruct, FilterProp, RowData, FilterValue) && EvaluateNumeric(FilterValue, Op, CompareValue))
        {
            FStringArray Out;
            Out.Values.Reserve(Columns.Num());
            for (const FProperty* ColProp : ColumnProps)
            {
                FString Cell;
                if (ColProp && TryGetFieldAsString(RowStruct, ColProp, RowData, Cell))
                {
                    Out.Values.Add(Cell);
                }
                else
                {
                    Out.Values.Add(TEXT(""));
                }
            }
            Results.Add(MoveTemp(Out));
        }
    }
    return Results;
}

TArray<FStringArray> UDatabaseRetrievalLibrary::RetrieveColumnsByStringCondition(UDataTable* Table, const TArray<FName>& Columns, FName FilterField, EStringCompareOp Op, const FString& CompareValue, bool bCaseSensitive)
{
    TArray<FStringArray> Results;
    if (!Table)
    {
        UE_LOG(LogTemp, Error, TEXT("[DatabaseRetrieval] Table is null"));
        return Results;
    }
    const UScriptStruct* RowStruct = Table->GetRowStruct();
    const FProperty* FilterProp = FindFieldChecked(RowStruct, FilterField);
    if (!FilterProp)
    {
        UE_LOG(LogTemp, Error, TEXT("[DatabaseRetrieval] Filter field %s not found in row struct %s"), *FilterField.ToString(), *GetNameSafe(RowStruct));
        return Results;
    }

    // Cache column properties
    TArray<const FProperty*> ColumnProps; ColumnProps.Reserve(Columns.Num());
    for (const FName& Col : Columns)
    {
        ColumnProps.Add(FindFieldChecked(RowStruct, Col));
    }

    for (const TPair<FName, uint8*>& Pair : Table->GetRowMap())
    {
        const void* RowData = Pair.Value;
        FString FilterValue;
        if (TryGetStringFieldValue(RowStruct, FilterProp, RowData, FilterValue) && EvaluateString(FilterValue, Op, CompareValue, bCaseSensitive))
        {
            FStringArray Out;
            Out.Values.Reserve(Columns.Num());
            for (const FProperty* ColProp : ColumnProps)
            {
                FString Cell;
                if (ColProp && TryGetFieldAsString(RowStruct, ColProp, RowData, Cell))
                {
                    Out.Values.Add(Cell);
                }
                else
                {
                    Out.Values.Add(TEXT(""));
                }
            }
            Results.Add(MoveTemp(Out));
        }
    }
    return Results;
}

