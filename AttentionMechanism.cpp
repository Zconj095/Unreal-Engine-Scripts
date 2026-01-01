#include "AttentionMechanism.h"
#include "Math/UnrealMathUtility.h"

float FAttentionMechanism::DotProduct(const TArray<float>& A, const TArray<float>& B)
{
    const int32 N = FMath::Min(A.Num(), B.Num());
    double Acc = 0.0;
    for (int32 i = 0; i < N; ++i)
    {
        Acc += static_cast<double>(A[i]) * static_cast<double>(B[i]);
    }
    return static_cast<float>(Acc);
}

void FAttentionMechanism::SoftmaxStable(const TArray<double>& InScores, TArray<float>& OutWeights)
{
    OutWeights.SetNum(InScores.Num());
    if (InScores.Num() == 0)
    {
        return;
    }

    double MaxScore = InScores[0];
    for (int32 i = 1; i < InScores.Num(); ++i)
    {
        MaxScore = FMath::Max(MaxScore, InScores[i]);
    }

    double Sum = 0.0;
    TArray<double> ExpScores;
    ExpScores.SetNumUninitialized(InScores.Num());
    for (int32 i = 0; i < InScores.Num(); ++i)
    {
        const double e = FMath::Exp(InScores[i] - MaxScore);
        ExpScores[i] = e;
        Sum += e;
    }
    if (Sum <= 0.0)
    {
        const float Uniform = 1.0f / FMath::Max(1, InScores.Num());
        for (int32 i = 0; i < InScores.Num(); ++i)
        {
            OutWeights[i] = Uniform;
        }
        return;
    }
    for (int32 i = 0; i < InScores.Num(); ++i)
    {
        OutWeights[i] = static_cast<float>(ExpScores[i] / Sum);
    }
}

TArray<float> FAttentionMechanism::ComputeAttention(
    const TArray<float>& Query,
    const TArray<TArray<float>>& Keys,
    const TArray<TArray<float>>& Values)
{
    const int32 KeyCount = Keys.Num();
    if (KeyCount <= 0)
    {
        return {};
    }
    const int32 KeyDim = Keys[0].Num();
    const int32 ValueDim = Values.Num() > 0 ? Values[0].Num() : 0;

    // 1) scores[i] = dot(Query, Keys[i])
    TArray<double> Scores;
    Scores.SetNumUninitialized(KeyCount);
    for (int32 i = 0; i < KeyCount; ++i)
    {
        Scores[i] = static_cast<double>(DotProduct(Query, Keys[i]));
    }

    // 2) attention weights = softmax(scores)
    TArray<float> Attn;
    SoftmaxStable(Scores, Attn);

    // 3) output[j] = sum_i Attn[i] * Values[i][j]
    TArray<float> Output;
    Output.Init(0.0f, ValueDim);
    for (int32 i = 0; i < Values.Num(); ++i)
    {
        const float Wi = Attn.IsValidIndex(i) ? Attn[i] : 0.0f;
        const TArray<float>& VRow = Values[i];
        for (int32 j = 0; j < VRow.Num(); ++j)
        {
            if (!Output.IsValidIndex(j))
            {
                Output.AddZeroed(j - Output.Num() + 1);
            }
            Output[j] += Wi * VRow[j];
        }
    }

    return Output;
}

