#include "LabelingMultidimensionalProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingMultidimensionalProcessorComponent::ULabelingMultidimensionalProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingMultidimensionalProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Compute joint probabilities and log
    const TArray<float> Joint = ComputeJointProbabilitiesFlat(
        DemoDataFlat, DemoRows, DemoDims,
        DemoTensorFlat, DemoNumCategories);

    FString JointStr;
    for (int32 r = 0; r < DemoRows; ++r)
    {
        JointStr += TEXT("[");
        for (int32 k = 0; k < DemoNumCategories; ++k)
        {
            const int32 idx = r * DemoNumCategories + k;
            JointStr += FString::SanitizeFloat(Joint.IsValidIndex(idx) ? Joint[idx] : 0.f);
            if (k + 1 < DemoNumCategories) JointStr += TEXT(", ");
        }
        JointStr += TEXT("] ");
    }
    UE_LOG(LogTemp, Log, TEXT("Joint Probabilities: %s"), *JointStr);

    // Assign labels from joint probabilities
    const TArray<int32> Labels = AssignLabelsFromJointProbabilitiesFlat(Joint, DemoRows, DemoNumCategories);
    FString LabelsStr;
    for (int32 i = 0; i < Labels.Num(); ++i)
    {
        LabelsStr += FString::FromInt(Labels[i]);
        if (i + 1 < Labels.Num()) LabelsStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels for Multidimensional Data: %s"), *LabelsStr);

    // Train tiny neural model
    TrainMultidimensionalLabelingNeuralNetwork();
}

TArray<float> ULabelingMultidimensionalProcessorComponent::ComputeJointProbabilitiesFlat(const TArray<float>& DataFlat, int32 Rows, int32 Dims,
                                                                                          const TArray<float>& TensorFlat, int32 NumCategories)
{
    TArray<float> Out;
    if (Rows <= 0 || Dims <= 0 || NumCategories <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("ComputeJointProbabilitiesFlat: invalid dims."));
        return Out;
    }
    if (DataFlat.Num() != Rows * Dims)
    {
        UE_LOG(LogTemp, Error, TEXT("ComputeJointProbabilitiesFlat: Data size %d != Rows*Dims %d."), DataFlat.Num(), Rows * Dims);
        return Out;
    }
    if (TensorFlat.Num() != NumCategories * Dims * Dims)
    {
        UE_LOG(LogTemp, Error, TEXT("ComputeJointProbabilitiesFlat: Tensor size %d != K*D*D %d."), TensorFlat.Num(), NumCategories * Dims * Dims);
        return Out;
    }

    Out.SetNumZeroed(Rows * NumCategories);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* X = DataFlat.GetData() + r * Dims;
        // Precompute sum_m x_m
        double SumX = 0.0;
        for (int32 m = 0; m < Dims; ++m)
        {
            SumX += static_cast<double>(X[m]);
        }

        for (int32 k = 0; k < NumCategories; ++k)
        {
            const float* Tk = TensorFlat.GetData() + k * Dims * Dims; // (i,j) block for category k
            double SumIJ = 0.0;
            for (int32 i = 0; i < Dims; ++i)
            {
                for (int32 j = 0; j < Dims; ++j)
                {
                    const float Tij = Tk[i * Dims + j];
                    SumIJ += static_cast<double>(Tij) * static_cast<double>(X[i]) * static_cast<double>(X[j]);
                }
            }
            const double Joint = SumIJ * SumX; // equivalent to triple-sum with tensor independent of m
            Out[r * NumCategories + k] = static_cast<float>(Joint);
        }
    }
    return Out;
}

TArray<int32> ULabelingMultidimensionalProcessorComponent::AssignLabelsFromJointProbabilitiesFlat(const TArray<float>& JointFlat, int32 Rows, int32 NumCategories)
{
    TArray<int32> Labels;
    if (Rows <= 0 || NumCategories <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsFromJointProbabilitiesFlat: invalid dims."));
        return Labels;
    }
    if (JointFlat.Num() != Rows * NumCategories)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsFromJointProbabilitiesFlat: size %d != Rows*NumCategories %d."), JointFlat.Num(), Rows * NumCategories);
        return Labels;
    }

    Labels.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* RowPtr = JointFlat.GetData() + r * NumCategories;
        Labels[r] = ArgMax(RowPtr, NumCategories);
    }
    return Labels;
}

int32 ULabelingMultidimensionalProcessorComponent::ArgMax(const float* Data, int32 Count)
{
    if (Count <= 0) return 0;
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

// -------------------- Simple 4-8-3 MLP --------------------

ULabelingMultidimensionalProcessorComponent::FMLP483::FMLP483()
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

float ULabelingMultidimensionalProcessorComponent::FMLP483::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingMultidimensionalProcessorComponent::FMLP483::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingMultidimensionalProcessorComponent::FMLP483::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingMultidimensionalProcessorComponent::FMLP483::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingMultidimensionalProcessorComponent::TrainMultidimensionalLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 4 features; Outputs: 3 probabilities
    const float Inputs[3][4] = {
        { 0.5f, 0.3f, 0.2f, 0.1f },
        { 0.7f, 0.6f, 0.5f, 0.4f },
        { 0.9f, 0.8f, 0.7f, 0.6f }
    };
    const float Targets[3][3] = {
        { 0.9f, 0.2f, 0.1f },
        { 0.4f, 0.6f, 0.2f },
        { 0.7f, 0.3f, 0.8f }
    };
    const int32 SampleCount = 3;

    FMLP483 Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP483::HiddenSize];
            float Output[FMLP483::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            for (int32 o = 0; o < FMLP483::OutSize; ++o)
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

