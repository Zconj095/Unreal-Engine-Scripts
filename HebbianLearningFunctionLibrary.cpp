#include "HebbianLearningFunctionLibrary.h"

void UHebbianLearningFunctionLibrary::ApplyHebbianLearning(TMap<FHebbianConnectionKey, float>& Connections, FName NodeA, FName NodeB, float ActivationA, float ActivationB)
{
	FHebbianConnectionKey Key;
	Key.NodeA = NodeA;
	Key.NodeB = NodeB;

	if (float* Value = Connections.Find(Key))
	{
		*Value += ActivationA * ActivationB;
		*Value = FMath::Clamp(*Value, 0.f, 1.f);
	}
}
