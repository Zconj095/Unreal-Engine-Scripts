#include "MultiLayeredEmbedding.h"

void UMultiLayeredEmbedding::Initialize(const TArray<float>& InBaseEmbedding)
{
    BaseEmbedding = InBaseEmbedding;
    Layers.Reset();
}

bool UMultiLayeredEmbedding::AddLayer(const TArray<float>& Layer, FString& OutError)
{
    OutError.Reset();
    if (Layer.Num() != BaseEmbedding.Num())
    {
        OutError = FString::Printf(TEXT("Layer dimensions (%d) must match base embedding dimensions (%d)."), Layer.Num(), BaseEmbedding.Num());
        return false;
    }
    FFloatArray Wrap; Wrap.Values = Layer;
    Layers.Add(MoveTemp(Wrap));
    return true;
}

void UMultiLayeredEmbedding::GetHighDimensionalRepresentation(TArray<float>& OutCombined) const
{
    const int32 N = BaseEmbedding.Num();
    OutCombined = BaseEmbedding; // copy base
    if (N == 0 || Layers.Num() == 0)
    {
        return;
    }
    for (const FFloatArray& L : Layers)
    {
        const int32 M = FMath::Min(N, L.Values.Num());
        for (int32 i = 0; i < M; ++i)
        {
            OutCombined[i] += L.Values[i];
        }
    }
}

