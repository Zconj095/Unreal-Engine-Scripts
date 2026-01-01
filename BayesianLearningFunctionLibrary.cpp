#include "BayesianLearningFunctionLibrary.h"
#include "BayesianNetwork.h"

void UBayesianLearningFunctionLibrary::UpdateProbabilities(UBayesianNetwork* Network, const TMap<FString, float>& Evidence)
{
	if (!Network)
	{
		return;
	}

	Network->UpdateNetworkWithEvidence(Evidence);
}
