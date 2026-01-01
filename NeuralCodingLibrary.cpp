#include "NeuralCodingLibrary.h"

void UNeuralCodingLibrary::F(const TArray<float>& X, TArray<float>& OutY)
{
    OutY.SetNum(X.Num());
    for (int32 i = 0; i < X.Num(); ++i)
    {
        OutY[i] = 2.0f * X[i];
    }
}

void UNeuralCodingLibrary::F_inv(const TArray<float>& Y, TArray<float>& OutX)
{
    OutX.SetNum(Y.Num());
    for (int32 i = 0; i < Y.Num(); ++i)
    {
        OutX[i] = 0.5f * Y[i];
    }
}

void UNeuralCodingLibrary::EncodingWide(const TArray<float>& Stimuli, const TArray<float>& BodyState, const TArray<float>& BrainActivity, TArray<float>& OutEncoded)
{
    const int32 N = FMath::Min3(Stimuli.Num(), BodyState.Num(), BrainActivity.Num());
    OutEncoded.SetNum(N);
    for (int32 i = 0; i < N; ++i)
    {
        OutEncoded[i] = Stimuli[i] + BodyState[i] + BrainActivity[i];
    }
}

void UNeuralCodingLibrary::PredictError(const TArray<float>& CurrentState, const TArray<float>& PreviousState, TArray<float>& OutError)
{
    const int32 N = FMath::Min(CurrentState.Num(), PreviousState.Num());
    OutError.SetNum(N);
    for (int32 i = 0; i < N; ++i)
    {
        OutError[i] = CurrentState[i] - PreviousState[i];
    }
}

