#include "DecisionMaking.h"

FString FDecisionMaking::MakeDecision(const FString& Situation)
{
    if (Situation.Contains(TEXT("threat"), ESearchCase::IgnoreCase))
    {
        return TEXT("Prepare defense.");
    }
    if (Situation.Contains(TEXT("opportunity"), ESearchCase::IgnoreCase))
    {
        return TEXT("Take advantage.");
    }
    return TEXT("Analyze further.");
}

