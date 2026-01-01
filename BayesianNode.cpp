#include "BayesianNode.h"

void UBayesianNode::UpdateProbability(const FString& State, float Probability)
{
    Probabilities.Add(State, Probability);
}

float UBayesianNode::GetProbability(const FString& State) const
{
    if (const float* Found = Probabilities.Find(State))
    {
        return *Found;
    }
    return 0.0f;
}

