#include "LabelingComplexProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingComplexProcessorComponent::ULabelingComplexProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingComplexProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Nonlinear mapping with sigmoid
    const TArray<float> Transformed = NonlinearMappingFlatSigmoid(
        DemoDataPointsFlat, DemoDataRows, DemoDataCols,
        DemoWeightsFlat, DemoOutDim,
        DemoBias);

    // Log transformed data (Rows x OutDim)
    FString TransStr;
    for (int32 r = 0; r < DemoDataRows; ++r)
    {
        TransStr += TEXT("[");
        for (int32 c = 0; c < DemoOutDim; ++c)
        {
            const int32 idx = r * DemoOutDim + c;
            TransStr += FString::SanitizeFloat(Transformed.IsValidIndex(idx) ? Transformed[idx] : 0.f);
            if (c + 1 < DemoOutDim) TransStr += TEXT(", ");
        }
        TransStr += TEXT("] ");
    }
    UE_LOG(LogTemp, Log, TEXT("Transformed Data: %s"), *TransStr);

    // Assign labels to complex data using provided probabilities
    const TArray<int32> Labels = AssignLabelsToComplexDataFlat(DemoProbabilitiesFlat, DemoDataRows, DemoOutDim);
    FString LabelsStr;
    for (int32 i = 0; i < Labels.Num(); ++i)
    {
        LabelsStr += FString::FromInt(Labels[i]);
        if (i + 1 < Labels.Num()) LabelsStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels for Complex Data: %s"), *LabelsStr);

    // Train demo neural classifier
    TrainComplexDataLabelingNeuralNetwork();
}

TArray<float> ULabelingComplexProcessorComponent::NonlinearMappingFlatSigmoid(const TArray<float>& DataFlat, int32 Rows, int32 InDim,
                                                                              const TArray<float>& WeightsFlat, int32 OutDim,
                                                                              const TArray<float>& Bias)
{
    TArray<float> Out;
    if (Rows <= 0 || InDim <= 0 || OutDim <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("NonlinearMapping: Invalid dimensions."));
        return Out;
    }
    if (DataFlat.Num() != Rows * InDim)
    {
        UE_LOG(LogTemp, Error, TEXT("NonlinearMapping: Data size %d != Rows*InDim %d."), DataFlat.Num(), Rows * InDim);
        return Out;
    }
    if (WeightsFlat.Num() != OutDim * InDim)
    {
        UE_LOG(LogTemp, Error, TEXT("NonlinearMapping: Weights size %d != OutDim*InDim %d."), WeightsFlat.Num(), OutDim * InDim);
        return Out;
    }
    if (Bias.Num() != OutDim)
    {
        UE_LOG(LogTemp, Error, TEXT("NonlinearMapping: Bias size %d != OutDim %d."), Bias.Num(), OutDim);
        return Out;
    }

    Out.SetNumZeroed(Rows * OutDim);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* X = DataFlat.GetData() + r * InDim;
        for (int32 k = 0; k < OutDim; ++k)
        {
            const float* Wk = WeightsFlat.GetData() + k * InDim; // row-major (k, :)
            float Z = Bias[k];
            for (int32 c = 0; c < InDim; ++c)
            {
                Z += Wk[c] * X[c];
            }
            Out[r * OutDim + k] = 1.0f / (1.0f + FMath::Exp(-Z)); // sigmoid
        }
    }

    return Out;
}

TArray<int32> ULabelingComplexProcessorComponent::AssignLabelsToComplexDataFlat(const TArray<float>& ProbFlat, int32 Rows, int32 Cols)
{
    TArray<int32> Labels;
    if (Rows <= 0 || Cols <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToComplexDataFlat: Rows and Cols must be > 0."));
        return Labels;
    }
    if (ProbFlat.Num() != Rows * Cols)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToComplexDataFlat: Size %d != Rows*Cols %d."), ProbFlat.Num(), Rows * Cols);
        return Labels;
    }

    Labels.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* RowPtr = ProbFlat.GetData() + r * Cols;
        Labels[r] = ArgMax(RowPtr, Cols);
    }
    return Labels;
}

int32 ULabelingComplexProcessorComponent::ArgMax(const float* Data, int32 Count)
{
    if (Count <= 0)
    {
        return 0;
    }
    int32 BestIdx = 0;
    float BestVal = Data[0];
    for (int32 i = 1; i < Count; ++i)
    {
        if (Data[i] > BestVal)
        {
            BestVal = Data[i];
            BestIdx = i;
        }
    }
    return BestIdx;
}

// -------------------- Simple 3-6-2 MLP --------------------

ULabelingComplexProcessorComponent::FMLP362::FMLP362()
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

float ULabelingComplexProcessorComponent::FMLP362::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingComplexProcessorComponent::FMLP362::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingComplexProcessorComponent::FMLP362::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

    for (int32 o = 0; o < OutSize; ++o)
    {
        float Z = B2[o];
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            Z += W2[o][h] * OutHidden[h];
        }
        OutOutput[o] = Sigmoid(Z);
    }
}

void ULabelingComplexProcessorComponent::FMLP362::TrainSGD(const float* Input, const float* Target, float LearningRate)
{
    float Hidden[HiddenSize];
    float Output[OutSize];
    Forward(Input, Hidden, Output);

    float DeltaOut[OutSize];
    for (int32 o = 0; o < OutSize; ++o)
    {
        const float Err = Output[o] - Target[o];
        DeltaOut[o] = Err * SigmoidDerivFromY(Output[o]);
    }

    float DeltaHidden[HiddenSize] = {0};
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        float Grad = 0.0f;
        for (int32 o = 0; o < OutSize; ++o)
        {
            Grad += W2[o][h] * DeltaOut[o];
        }
        DeltaHidden[h] = Grad * SigmoidDerivFromY(Hidden[h]);
    }

    for (int32 o = 0; o < OutSize; ++o)
    {
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            W2[o][h] -= LearningRate * DeltaOut[o] * Hidden[h];
        }
        B2[o] -= LearningRate * DeltaOut[o];
    }

    for (int32 h = 0; h < HiddenSize; ++h)
    {
        for (int32 i = 0; i < InSize; ++i)
        {
            W1[h][i] -= LearningRate * DeltaHidden[h] * Input[i];
        }
        B1[h] -= LearningRate * DeltaHidden[h];
    }
}

void ULabelingComplexProcessorComponent::TrainComplexDataLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 3 features; Outputs: one-hot [C0, C1]
    const float Inputs[3][3] = {
        { 0.5f, 0.3f, 0.2f },
        { 0.7f, 0.6f, 0.5f },
      	{ 0.9f, 0.8f, 0.7f }
    };
    const float Targets[3][2] = {
        { 1.f, 0.f },
        { 0.f, 1.f },
        { 1.f, 0.f }
    };
    const int32 SampleCount = 3;

    FMLP362 Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP362::HiddenSize];
            float Output[FMLP362::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            for (int32 o = 0; o < FMLP362::OutSize; ++o)
            {
                const double Diff = static_cast<double>(Output[o]) - static_cast<double>(Targets[s][o]);
                EpochError += 0.5 * Diff * Diff;
            }
        }

        EpochError /= static_cast<double>(SampleCount);
        UE_LOG(LogTemp, Log, TEXT("Epoch %d, Error: %f"), Epoch, EpochError);
        if (EpochError < ErrorThreshold)
        {
            break;
        }
    }
}

