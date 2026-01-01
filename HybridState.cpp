#include "HybridState.h"

FHybridState FHybridState::Merge(const FHybridState& Other) const
{
    TSharedPtr<IHyperState> CombinedHyper;
    if (HyperState && Other.GetHyperState())
    {
        CombinedHyper = HyperState->Superpose(Other.GetHyperState());
    }

    ULLMQuantumState2* CombinedQuantum = nullptr;
    if (QuantumState && Other.GetQuantumState())
    {
        CombinedQuantum = QuantumState->Interfere(Other.GetQuantumState());
    }

    return FHybridState(CombinedHyper, CombinedQuantum);
}

float FHybridState::Classify() const
{
    if (!QuantumState)
    {
        return 0.f;
    }
    return QuantumState->Measure();
}
