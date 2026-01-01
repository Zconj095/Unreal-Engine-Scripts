#include "BayesianNetwork.h"

void UBayesianNetwork::UpdateNetworkWithEvidence(const TMap<FString, float>& Evidence)
{
	for (const TPair<FString, float>& Pair : Evidence)
	{
		NodeProbabilities.FindOrAdd(Pair.Key) = Pair.Value;
	}
}
