#include "LLMBayesianNetwork.h"

void ULLMBayesianNetwork::AddNode(UBayesianNode* Node)
{
    if (Node)
    {
        Nodes.Add(Node);
    }
}

void ULLMBayesianNetwork::UpdateBelief(const FString& NodeName, const FString& State, float Evidence)
{
    const TObjectPtr<UBayesianNode>* Found = Nodes.FindByPredicate([&NodeName](const TObjectPtr<UBayesianNode>& N)
    {
        return N && N->Name == NodeName;
    });
    if (!Found)
    {
        return;
    }

    UBayesianNode* Node = Found->Get();
    const float Prior = FMath::Clamp(Node->GetProbability(State), 0.0f, 1.0f);
    const float E = FMath::Clamp(Evidence, 0.0f, 1.0f);
    const float Den = (Prior * E) + ((1.0f - Prior) * (1.0f - E));
    if (Den <= KINDA_SMALL_NUMBER)
    {
        return; // avoid divide-by-zero; no update
    }
    const float Posterior = FMath::Clamp((Prior * E) / Den, 0.0f, 1.0f);
    Node->UpdateProbability(State, Posterior);
}

float ULLMBayesianNetwork::GetBelief(const FString& NodeName, const FString& State) const
{
    const TObjectPtr<UBayesianNode>* Found = Nodes.FindByPredicate([&NodeName](const TObjectPtr<UBayesianNode>& N)
    {
        return N && N->Name == NodeName;
    });
    if (!Found)
    {
        return 0.0f;
    }
    return Found->Get()->GetProbability(State);
}

void ULLMBayesianNetwork::GetNodes(TArray<UBayesianNode*>& OutNodes) const
{
    OutNodes.Reset();
    OutNodes.Reserve(Nodes.Num());
    for (UBayesianNode* Node : Nodes)
    {
        OutNodes.Add(Node);
    }
}
