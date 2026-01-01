#include "CyberneticFeedbackLibrary.h"

void UCyberneticFeedbackLibrary::AdjustWeights(const TArray<float>& Weights,
                                               const TArray<float>& Gradients,
                                               float LearningRate,
                                               TArray<float>& OutAdjusted,
                                               bool& bSuccess,
                                               FString& OutError)
{
    bSuccess = false;
    OutError.Reset();
    OutAdjusted.Reset();

    if (Weights.Num() == 0)
    {
        // Empty OK if gradients also empty; result remains empty
        if (Gradients.Num() == 0) { bSuccess = true; }
        else { OutError = TEXT("Gradients length does not match weights."); }
        return;
    }
    if (Weights.Num() != Gradients.Num())
    {
        OutError = FString::Printf(TEXT("Length mismatch: Weights=%d, Gradients=%d"), Weights.Num(), Gradients.Num());
        return;
    }

    OutAdjusted.SetNumUninitialized(Weights.Num());
    for (int32 i = 0; i < Weights.Num(); ++i)
    {
        OutAdjusted[i] = Weights[i] - LearningRate * Gradients[i];
    }

    bSuccess = true;
}

