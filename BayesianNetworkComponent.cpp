#include "BayesianNetworkComponent.h"
#include "EnviroHMMBayesianNode.h"

UBayesianNetworkComponent::UBayesianNetworkComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBayesianNetworkComponent::UpdateNetworkWithEvidence(const TMap<FString, float>& Evidence)
{
	for (UEnviroHMMBayesianNode* Node : Nodes)
	{
		if (Node && Evidence.Contains(Node->NodeName))
		{
			Node->UpdateProbability(Evidence[Node->NodeName]);
		}
	}
}
