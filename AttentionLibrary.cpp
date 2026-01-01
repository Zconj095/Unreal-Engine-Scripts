#include "AttentionLibrary.h"
#include "Math/UnrealMathUtility.h"

float UAttentionLibrary::DotProduct(const float* A, const float* B, int32 N)
{
    double Acc = 0.0;
    for (int32 i = 0; i < N; ++i)
    {
        Acc += static_cast<double>(A[i]) * static_cast<double>(B[i]);
    }
    return static_cast<float>(Acc);
}

void UAttentionLibrary::SoftmaxStable(const TArray<double>& InScores, TArray<float>& OutWeights)
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
        // fallback to uniform
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

void UAttentionLibrary::ComputeAttentionFlattened(
    const TArray<float>& Query,
    const TArray<float>& Keys,
    int32 KeyCount,
    int32 KeyDim,
    const TArray<float>& Values,
    int32 ValueCount,
    int32 ValueDim,
    TArray<float>& OutOutput,
    bool& bSuccess,
    FString& OutError)
{
    bSuccess = false;
    OutError.Reset();
    OutOutput.Reset();

    if (KeyCount <= 0 || KeyDim <= 0 || ValueCount <= 0 || ValueDim <= 0)
    {
        OutError = TEXT("Invalid dimensions; counts and dims must be > 0.");
        return;
    }
    if (Query.Num() != KeyDim)
    {
        OutError = FString::Printf(TEXT("Query length %d != KeyDim %d"), Query.Num(), KeyDim);
        return;
    }
    if (Keys.Num() != KeyCount * KeyDim)
    {
        OutError = FString::Printf(TEXT("Keys length %d != KeyCount*KeyDim %d"), Keys.Num(), KeyCount * KeyDim);
        return;
    }
    if (ValueCount != KeyCount)
    {
        OutError = FString::Printf(TEXT("ValueCount %d != KeyCount %d"), ValueCount, KeyCount);
        return;
    }
    if (Values.Num() != ValueCount * ValueDim)
    {
        OutError = FString::Printf(TEXT("Values length %d != ValueCount*ValueDim %d"), Values.Num(), ValueCount * ValueDim);
        return;
    }

    // 1) scores[i] = dot(Query, Key_i)
    TArray<double> Scores;
    Scores.SetNumUninitialized(KeyCount);
    for (int32 i = 0; i < KeyCount; ++i)
    {
        const float* KeyPtr = Keys.GetData() + i * KeyDim;
        Scores[i] = static_cast<double>(DotProduct(Query.GetData(), KeyPtr, KeyDim));
    }

    // 2) attention weights = softmax(scores)
    TArray<float> Attn;
    SoftmaxStable(Scores, Attn);

    // 3) output[j] = sum_i Attn[i] * Values[i, j]
    OutOutput.SetNumZeroed(ValueDim);
    for (int32 i = 0; i < ValueCount; ++i)
    {
        const float Wi = Attn[i];
        const float* VRow = Values.GetData() + i * ValueDim;
        for (int32 j = 0; j < ValueDim; ++j)
        {
            OutOutput[j] += Wi * VRow[j];
        }
    }

    bSuccess = true;
}

void UAttentionLibrary::FeedForward(
    const TArray<float>& Input,
    const TArray<float>& Weights,
    int32 InDim,
    int32 OutDim,
    const TArray<float>& Biases,
    TArray<float>& OutOutput,
    bool& bSuccess,
    FString& OutError)
{
    bSuccess = false;
    OutError.Reset();
    OutOutput.Reset();

    if (InDim <= 0 || OutDim <= 0)
    {
        OutError = TEXT("Invalid dimensions; InDim and OutDim must be > 0.");
        return;
    }
    if (Input.Num() != InDim)
    {
        OutError = FString::Printf(TEXT("Input length %d != InDim %d"), Input.Num(), InDim);
        return;
    }
    if (Weights.Num() != InDim * OutDim)
    {
        OutError = FString::Printf(TEXT("Weights length %d != InDim*OutDim %d"), Weights.Num(), InDim * OutDim);
        return;
    }
    if (Biases.Num() != OutDim)
    {
        OutError = FString::Printf(TEXT("Biases length %d != OutDim %d"), Biases.Num(), OutDim);
        return;
    }

    OutOutput.SetNumUninitialized(OutDim);
    for (int32 j = 0; j < OutDim; ++j)
    {
        double Acc = static_cast<double>(Biases[j]);
        for (int32 i = 0; i < InDim; ++i)
        {
            const float Wij = Weights[i * OutDim + j]; // weights stored row-major [InDim x OutDim]
            Acc += static_cast<double>(Input[i]) * static_cast<double>(Wij);
        }
        OutOutput[j] = static_cast<float>(Acc);
    }

    bSuccess = true;
}

