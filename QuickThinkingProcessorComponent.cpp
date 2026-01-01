#include "QuickThinkingProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

UQuickThinkingProcessorComponent::UQuickThinkingProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Default demo memory vectors
    DemoMemoryVectors.SetNum(3);
    DemoMemoryVectors[0].Values = {0.8f, 0.6f};
    DemoMemoryVectors[1].Values = {0.7f, 0.9f};
    DemoMemoryVectors[2].Values = {0.5f, 0.4f};
}

void UQuickThinkingProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Retrieval success Rt = M * exp(-alpha * t)
    const float Rt = CalculateRetrievalSuccess(DemoInitialMemoryStrength, DemoAlpha, DemoTime);
    UE_LOG(LogTemp, Log, TEXT("Retrieval Success (Rt): %f"), Rt);

    // Quickness Qt = Rt / dt
    const float Qt = CalculateQuicknessOfThinking(Rt, DemoDeltaTime);
    UE_LOG(LogTemp, Log, TEXT("Quickness of Thinking (Qt): %f"), Qt);

    // Associative retrieval with cosine similarity
    const float Ra = CalculateAssociativeRetrieval(DemoMemoryVectors, DemoQueryVector, DemoProbabilities);
    UE_LOG(LogTemp, Log, TEXT("Associative Retrieval Score (Ra): %f"), Ra);

    // Tiny neural demo
    TrainNeuralQuickThinking();
}

float UQuickThinkingProcessorComponent::CalculateRetrievalSuccess(float InitialMemoryStrength, float Alpha, float Time)
{
    return InitialMemoryStrength * FMath::Exp(-Alpha * Time);
}

float UQuickThinkingProcessorComponent::CalculateQuicknessOfThinking(float RetrievedInformation, float DeltaTime)
{
    if (FMath::IsNearlyZero(DeltaTime))
    {
        UE_LOG(LogTemp, Warning, TEXT("DeltaTime is zero; returning 0."));
        return 0.0f;
    }
    return RetrievedInformation / DeltaTime;
}

float UQuickThinkingProcessorComponent::CalculateAssociativeRetrieval(const TArray<FFloatArray>& MemoryVectors,
                                                                      const TArray<float>& QueryVector,
                                                                      const TArray<float>& Probabilities)
{
    const int32 N = MemoryVectors.Num();
    if (N == 0)
    {
        return 0.0f;
    }
    if (Probabilities.Num() != N)
    {
        UE_LOG(LogTemp, Error, TEXT("CalculateAssociativeRetrieval: Probabilities length %d != MemoryVectors length %d."), Probabilities.Num(), N);
        return 0.0f;
    }
    const int32 D = QueryVector.Num();
    if (D == 0)
    {
        return 0.0f;
    }

    // Precompute query norm
    const float* Q = QueryVector.GetData();
    const float Qn = FMath::Max(1e-6f, Norm(Q, D));

    double Acc = 0.0;
    for (int32 i = 0; i < N; ++i)
    {
        const TArray<float>& Mi = MemoryVectors[i].Values;
        if (Mi.Num() != D)
        {
            UE_LOG(LogTemp, Warning, TEXT("Memory vector %d has dim %d != Query dim %d; skipping."), i, Mi.Num(), D);
            continue;
        }
        const float* M = Mi.GetData();
        const float Mn = FMath::Max(1e-6f, Norm(M, D));
        const float Cos = Dot(M, Q, D) / (Mn * Qn);
        Acc += static_cast<double>(Cos) * static_cast<double>(Probabilities[i]);
    }

    return static_cast<float>(Acc);
}

float UQuickThinkingProcessorComponent::Dot(const float* A, const float* B, int32 N)
{
    double s = 0.0;
    for (int32 i = 0; i < N; ++i)
    {
        s += static_cast<double>(A[i]) * static_cast<double>(B[i]);
    }
    return static_cast<float>(s);
}

float UQuickThinkingProcessorComponent::Norm(const float* A, int32 N)
{
    double s = 0.0;
    for (int32 i = 0; i < N; ++i)
    {
        s += static_cast<double>(A[i]) * static_cast<double>(A[i]);
    }
    return static_cast<float>(FMath::Sqrt(s));
}

// -------------------- Simple 2-3-1 MLP --------------------

UQuickThinkingProcessorComponent::FMLP231::FMLP231()
{
    const float Scale = 0.1f;
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        for (int32 i = 0; i < InSize; ++i)
        {
            W1[h][i] = FMath::FRandRange(-Scale, Scale);
        }
        B1[h] = FMath::FRandRange(-Scale, Scale);
    }
    for (int32 o = 0; o < OutSize; ++o)
    {
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            W2[o][h] = FMath::FRandRange(-Scale, Scale);
        }
        B2[o] = FMath::FRandRange(-Scale, Scale);
    }
}

float UQuickThinkingProcessorComponent::FMLP231::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float UQuickThinkingProcessorComponent::FMLP231::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void UQuickThinkingProcessorComponent::FMLP231::Forward(const float* Input, float* OutHidden, float* OutOutput) const
{
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        float Z = B1[h];
        for (int32 i = 0; i < InSize; ++i)
        {
            Z += W1[h][i] * Input[i];
        }
        OutHidden[h] = Sigmoid(Z);
    }

    float Zout = B2[0];
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        Zout += W2[0][h] * OutHidden[h];
    }
    OutOutput[0] = Sigmoid(Zout);
}

void UQuickThinkingProcessorComponent::FMLP231::TrainSGD(const float* Input, const float* Target, float LearningRate)
{
    float Hidden[HiddenSize];
    float Output[OutSize];
    Forward(Input, Hidden, Output);

    const float Error = Output[0] - Target[0];
    const float DeltaOut = Error * SigmoidDerivFromY(Output[0]);

    float DeltaHidden[HiddenSize];
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        const float Grad = W2[0][h] * DeltaOut;
        DeltaHidden[h] = Grad * SigmoidDerivFromY(Hidden[h]);
    }

    for (int32 h = 0; h < HiddenSize; ++h)
    {
        W2[0][h] -= LearningRate * DeltaOut * Hidden[h];
    }
    B2[0] -= LearningRate * DeltaOut;

    for (int32 h = 0; h < HiddenSize; ++h)
    {
        for (int32 i = 0; i < InSize; ++i)
        {
            W1[h][i] -= LearningRate * DeltaHidden[h] * Input[i];
        }
        B1[h] -= LearningRate * DeltaHidden[h];
    }
}

void UQuickThinkingProcessorComponent::TrainNeuralQuickThinking(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: [alpha, t], Output: Rt (toy values from example)
    const float Inputs[3][2] = {
        { 0.1f, 1.0f },
        { 0.2f, 2.0f },
        { 0.3f, 3.0f }
    };
    const float Targets[3] = { 0.9f, 0.8f, 0.7f };
    const int32 SampleCount = 3;

    FMLP231 Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            const float T = Targets[s];
            Net.TrainSGD(Inputs[s], &T, LearningRate);

            float Hidden[FMLP231::HiddenSize];
            float Output[FMLP231::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            const double Diff = static_cast<double>(Output[0]) - static_cast<double>(T);
            EpochError += 0.5 * Diff * Diff;
        }

        EpochError /= static_cast<double>(SampleCount);
        UE_LOG(LogTemp, Log, TEXT("Epoch %d, Error: %f"), Epoch, EpochError);
        if (EpochError < ErrorThreshold)
        {
            break;
        }
    }
}
