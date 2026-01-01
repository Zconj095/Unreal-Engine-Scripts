#include "LLMCrossCorrelationLibrary.h"

void ULLMCrossCorrelationLibrary::CorrelateStates(UCrossDimensionalVector* Vector, FFloatCorrelation Correlation, TArray<float>& OutCorrelated)
{
    OutCorrelated.Reset();
    if (!Vector)
    {
        return;
    }
    Vector->CrossReferenceCustom(Correlation, OutCorrelated);
}

