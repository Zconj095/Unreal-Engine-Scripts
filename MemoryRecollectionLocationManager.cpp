#include "MemoryRecollectionLocationManager.h"
#include "Math/UnrealMathUtility.h"

namespace
{
    FORCEINLINE int W1Index(int r, int c, int Hidden, int Input) { return r * Input + c; }
    FORCEINLINE int W2Index(int r, int c, int Output, int Hidden) { return r * Hidden + c; }
}

double UMemoryRecollectionLocationManager::Sigmoid(double x)
{
    x = FMath::Clamp(x, -50.0, 50.0);
    return 1.0 / (1.0 + FMath::Exp(-x));
}

double UMemoryRecollectionLocationManager::SigmoidDerivFromActivation(double a)
{
    return a * (1.0 - a);
}

void UMemoryRecollectionLocationManager::RandomizeParams(double Scale)
{
    W1.SetNumZeroed(HiddenSize * InputSize);
    b1.SetNumZeroed(HiddenSize);
    W2.SetNumZeroed(OutputSize * HiddenSize);
    b2.SetNumZeroed(OutputSize);

    for (int i = 0; i < W1.Num(); ++i) W1[i] = FMath::FRandRange(-Scale, Scale);
    for (int i = 0; i < b1.Num(); ++i) b1[i] = 0.0;
    for (int i = 0; i < W2.Num(); ++i) W2[i] = FMath::FRandRange(-Scale, Scale);
    for (int i = 0; i < b2.Num(); ++i) b2[i] = 0.0;
}

void UMemoryRecollectionLocationManager::InitializeNeuralNetwork(int32 InInputSize, int32 InHiddenNeurons, int32 InOutputSize)
{
    InputSize = InInputSize;
    HiddenSize = InHiddenNeurons;
    OutputSize = InOutputSize; // expected 2
    RandomizeParams(0.1);
}

void UMemoryRecollectionLocationManager::AddRecollectionEntry(const FString& Id, const FVector2D& Location, const TArray<double>& Features, float RecollectionTimeSec)
{
    Entries.Add(FRecollectionLocationEntry(Id, Location, Features, RecollectionTimeSec));
}

double UMemoryRecollectionLocationManager::CalculateAverageRecollectionTime() const
{
    if (Entries.Num() == 0) return 0.0;
    double Sum = 0.0;
    for (const auto& E : Entries) { Sum += static_cast<double>(E.RecollectionTimeSec); }
    return Sum / static_cast<double>(Entries.Num());
}

FVector2D UMemoryRecollectionLocationManager::CalculateAverageLocation() const
{
    if (Entries.Num() == 0) return FVector2D::ZeroVector;
    double X = 0.0, Y = 0.0;
    for (const auto& E : Entries) { X += E.Location.X; Y += E.Location.Y; }
    const double N = static_cast<double>(Entries.Num());
    return FVector2D(static_cast<float>(X / N), static_cast<float>(Y / N));
}

void UMemoryRecollectionLocationManager::ForwardSingle(const double* InX, TArray<double>& Z1, TArray<double>& A1, TArray<double>& Z2, TArray<double>& A2) const
{
    Z1.SetNumUninitialized(HiddenSize);
    A1.SetNumUninitialized(HiddenSize);
    for (int h = 0; h < HiddenSize; ++h)
    {
        double z = b1[h];
        for (int i = 0; i < InputSize; ++i)
        {
            z += W1[W1Index(h, i, HiddenSize, InputSize)] * InX[i];
        }
        Z1[h] = z;
        A1[h] = Sigmoid(z);
    }

    Z2.SetNumUninitialized(OutputSize);
    A2.SetNumUninitialized(OutputSize);
    for (int o = 0; o < OutputSize; ++o)
    {
        double z = b2[o];
        for (int h = 0; h < HiddenSize; ++h)
        {
            z += W2[W2Index(o, h, OutputSize, HiddenSize)] * A1[h];
        }
        Z2[o] = z;
        A2[o] = Sigmoid(z);
    }
}

double UMemoryRecollectionLocationManager::TrainNetwork(int32 Epochs, double LearningRate)
{
    if (Entries.Num() == 0 || InputSize == 0 || HiddenSize == 0 || OutputSize == 0)
    {
        return 0.0;
    }

    const int32 N = Entries.Num();
    TArray<double> X; X.SetNumUninitialized(N * InputSize);
    TArray<double> Y; Y.SetNumUninitialized(N * OutputSize); // 2 outputs

    for (int32 n = 0; n < N; ++n)
    {
        const FRecollectionLocationEntry& Dpt = Entries[n];
        for (int32 i = 0; i < InputSize; ++i)
        {
            X[n * InputSize + i] = (i < Dpt.Features.Num()) ? Dpt.Features[i] : 0.0;
        }
        // Targets: X, Y
        Y[n * OutputSize + 0] = static_cast<double>(Dpt.Location.X);
        if (OutputSize > 1) Y[n * OutputSize + 1] = static_cast<double>(Dpt.Location.Y);
    }

    TArray<double> Z1, A1, Z2, A2;
    TArray<double> dW1, db1g, dW2, db2g;
    dW1.SetNumZeroed(W1.Num());
    db1g.SetNumZeroed(b1.Num());
    dW2.SetNumZeroed(W2.Num());
    db2g.SetNumZeroed(b2.Num());

    double FinalError = 0.0;
    for (int32 epoch = 0; epoch < Epochs; ++epoch)
    {
        FMemory::Memzero(dW1.GetData(), sizeof(double) * dW1.Num());
        FMemory::Memzero(db1g.GetData(), sizeof(double) * db1g.Num());
        FMemory::Memzero(dW2.GetData(), sizeof(double) * dW2.Num());
        FMemory::Memzero(db2g.GetData(), sizeof(double) * db2g.Num());

        double SumSqError = 0.0;

        for (int32 n = 0; n < N; ++n)
        {
            const double* x = &X[n * InputSize];
            const double* y = &Y[n * OutputSize];

            ForwardSingle(x, Z1, A1, Z2, A2);

            // Output layer delta
            TArray<double> dA2; dA2.SetNumUninitialized(OutputSize);
            for (int o = 0; o < OutputSize; ++o)
            {
                const double err = A2[o] - y[o];
                SumSqError += 0.5 * err * err;
                dA2[o] = err * SigmoidDerivFromActivation(A2[o]);
            }

            // Gradients W2, b2
            for (int o = 0; o < OutputSize; ++o)
            {
                db2g[o] += dA2[o];
                for (int h = 0; h < HiddenSize; ++h)
                {
                    dW2[W2Index(o, h, OutputSize, HiddenSize)] += dA2[o] * A1[h];
                }
            }

            // Backprop to hidden
            TArray<double> dA1; dA1.SetNumUninitialized(HiddenSize);
            for (int h = 0; h < HiddenSize; ++h)
            {
                double grad = 0.0;
                for (int o = 0; o < OutputSize; ++o)
                {
                    grad += dA2[o] * W2[W2Index(o, h, OutputSize, HiddenSize)];
                }
                dA1[h] = grad * SigmoidDerivFromActivation(A1[h]);
            }

            // Gradients W1, b1
            for (int h = 0; h < HiddenSize; ++h)
            {
                db1g[h] += dA1[h];
                for (int i = 0; i < InputSize; ++i)
                {
                    dW1[W1Index(h, i, HiddenSize, InputSize)] += dA1[h] * x[i];
                }
            }
        }

        const double InvN = 1.0 / FMath::Max(1, N);
        for (int i = 0; i < dW1.Num(); ++i) W1[i] -= LearningRate * dW1[i] * InvN;
        for (int i = 0; i < db1g.Num(); ++i) b1[i] -= LearningRate * db1g[i] * InvN;
        for (int i = 0; i < dW2.Num(); ++i) W2[i] -= LearningRate * dW2[i] * InvN;
        for (int i = 0; i < db2g.Num(); ++i) b2[i] -= LearningRate * db2g[i] * InvN;

        FinalError = SumSqError * InvN;
        if (epoch % 100 == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Epoch %d: Error = %f"), epoch, FinalError);
        }
    }

    return FinalError;
}

FVector2D UMemoryRecollectionLocationManager::PredictRecollectionLocation(const TArray<double>& Features) const
{
    if (InputSize == 0 || HiddenSize == 0 || OutputSize == 0) return FVector2D::ZeroVector;

    TArray<double> In; In.SetNumZeroed(InputSize);
    for (int32 i = 0; i < InputSize && i < Features.Num(); ++i) In[i] = Features[i];

    TArray<double> Z1, A1, Z2, A2;
    ForwardSingle(In.GetData(), Z1, A1, Z2, A2);

    const double X = (OutputSize >= 1) ? A2[0] : 0.0;
    const double Y = (OutputSize >= 2) ? A2[1] : 0.0;
    return FVector2D(static_cast<float>(X), static_cast<float>(Y));
}

