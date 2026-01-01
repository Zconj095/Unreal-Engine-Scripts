#include "CategorizedDataRetrievalLibrary.h"

TArray<FDoubleArray> UCategorizedDataRetrievalLibrary::RetrieveFromCategory(const FDoubleMatrix& Category, int32 ColumnIndex, ENumericCompareOp Op, double CompareValue)
{
    TArray<FDoubleArray> Results;
    for (const FDoubleArray& Row : Category.Rows)
    {
        if (!Row.Values.IsValidIndex(ColumnIndex))
        {
            continue;
        }
        if (Evaluate(Row.Values[ColumnIndex], Op, CompareValue))
        {
            Results.Add(Row);
        }
    }
    return Results;
}

TArray<FDoubleArray> UCategorizedDataRetrievalLibrary::RetrieveFromMultipleCategories(const TArray<FDoubleMatrix>& Categories, int32 ColumnIndex, ENumericCompareOp Op, double CompareValue)
{
    TArray<FDoubleArray> Results;
    for (const FDoubleMatrix& Category : Categories)
    {
        Results.Append(RetrieveFromCategory(Category, ColumnIndex, Op, CompareValue));
    }
    return Results;
}

FActivationNetworkStub UCategorizedDataRetrievalLibrary::TrainCategorizedConditionNeuralNetwork(const TArray<FDoubleArray>& InputData, const TArray<FDoubleArray>& OutputLabels, int32 Epochs, double LearningRate)
{
    FActivationNetworkStub Net;
    if (InputData.Num() == 0 || OutputLabels.Num() == 0 || InputData.Num() != OutputLabels.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("[CategorizedDataRetrieval] Invalid training data sizes."));
        return Net;
    }

    const int32 N = InputData.Num();
    const int32 InputSize = InputData[0].Values.Num();
    const int32 OutputSize = OutputLabels[0].Values.Num();

    // Initialize network
    Net.InputSize = InputSize;
    Net.OutputSize = OutputSize;
    Net.Weights.SetNum(OutputSize);
    Net.Biases.SetNumZeroed(OutputSize);
    for (int32 o = 0; o < OutputSize; ++o)
    {
        Net.Weights[o].Values.SetNum(InputSize);
        for (int32 i = 0; i < InputSize; ++i)
        {
            // Small random init around zero
            Net.Weights[o].Values[i] = FMath::FRandRange(-0.01, 0.01);
        }
        Net.Biases[o] = 0.0;
    }

    // Simple batch gradient descent with sigmoid + MSE
    for (int32 epoch = 0; epoch < Epochs; ++epoch)
    {
        double TotalError = 0.0;

        // Accumulators
        TArray<FDoubleArray> dW; dW.SetNum(OutputSize);
        TArray<double> dB; dB.SetNumZeroed(OutputSize);
        for (int32 o = 0; o < OutputSize; ++o)
        {
            dW[o].Values.Init(0.0, InputSize);
        }

        for (int32 n = 0; n < N; ++n)
        {
            const TArray<double>& X = InputData[n].Values;
            const TArray<double>& Y = OutputLabels[n].Values;

            // Forward: compute outputs
            TArray<double> Z; Z.SetNum(OutputSize);
            TArray<double> A; A.SetNum(OutputSize);
            for (int32 o = 0; o < OutputSize; ++o)
            {
                double z = Net.Biases[o];
                for (int32 i = 0; i < InputSize; ++i)
                {
                    z += Net.Weights[o].Values[i] * X[i];
                }
                Z[o] = z;
                A[o] = Sigmoid(z);
            }

            // Error and gradients for MSE: dL/dz = (a - y) * a * (1 - a)
            for (int32 o = 0; o < OutputSize; ++o)
            {
                const double diff = A[o] - Y[o];
                const double dz = diff * A[o] * (1.0 - A[o]);
                TotalError += 0.5 * diff * diff;
                for (int32 i = 0; i < InputSize; ++i)
                {
                    dW[o].Values[i] += dz * X[i];
                }
                dB[o] += dz;
            }
        }

        // Average gradients and update
        const double InvN = 1.0 / static_cast<double>(N);
        for (int32 o = 0; o < OutputSize; ++o)
        {
            for (int32 i = 0; i < InputSize; ++i)
            {
                Net.Weights[o].Values[i] -= LearningRate * (dW[o].Values[i] * InvN);
            }
            Net.Biases[o] -= LearningRate * (dB[o] * InvN);
        }

        if ((epoch % 50) == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Epoch %d: Error = %f"), epoch, (float)(TotalError * InvN));
        }

        if (TotalError * InvN < 0.001)
        {
            break;
        }
    }

    return Net;
}

FDoubleArray UCategorizedDataRetrievalLibrary::ComputeActivationNetwork(const FActivationNetworkStub& Network, const FDoubleArray& Input)
{
    FDoubleArray Out; Out.Values.SetNum(Network.OutputSize);
    if (Input.Values.Num() != Network.InputSize)
    {
        UE_LOG(LogTemp, Error, TEXT("[CategorizedDataRetrieval] Input size mismatch: expected %d, got %d"), Network.InputSize, Input.Values.Num());
        return Out;
    }
    for (int32 o = 0; o < Network.OutputSize; ++o)
    {
        double z = Network.Biases.IsValidIndex(o) ? Network.Biases[o] : 0.0;
        for (int32 i = 0; i < Network.InputSize; ++i)
        {
            z += Network.Weights[o].Values.IsValidIndex(i) ? (Network.Weights[o].Values[i] * Input.Values[i]) : 0.0;
        }
        Out.Values[o] = Sigmoid(z);
    }
    return Out;
}

void UCategorizedDataRetrievalLibrary::ExampleUsage()
{
    // Build example categories
    FDoubleMatrix Cat1; Cat1.Rows.SetNum(2);
    Cat1.Rows[0].Values = { 1.0, 30.0, 1.0 };
    Cat1.Rows[1].Values = { 2.0, 25.0, 0.0 };

    FDoubleMatrix Cat2; Cat2.Rows.SetNum(2);
    Cat2.Rows[0].Values = { 3.0, 35.0, 1.0 };
    Cat2.Rows[1].Values = { 4.0, 28.0, 0.0 };

    TArray<FDoubleMatrix> Categories; Categories.Add(Cat1); Categories.Add(Cat2);

    // Retrieve rows where Age > 30 from category 1 (Age at column index 1)
    {
        const TArray<FDoubleArray> Result = RetrieveFromCategory(Cat1, 1, ENumericCompareOp::Greater, 30.0);
        UE_LOG(LogTemp, Log, TEXT("Rows from Category 1 where Age > 30:"));
        for (const FDoubleArray& Row : Result)
        {
            FString S; for (int32 i=0;i<Row.Values.Num();++i){ if(i) S+=TEXT(", "); S+= FString::SanitizeFloat((float)Row.Values[i]); }
            UE_LOG(LogTemp, Log, TEXT("[%s]"), *S);
        }
    }

    // Retrieve rows where Active == 1 from all categories (Active at column index 2)
    {
        const TArray<FDoubleArray> Result = RetrieveFromMultipleCategories(Categories, 2, ENumericCompareOp::Equal, 1.0);
        UE_LOG(LogTemp, Log, TEXT("Rows from all categories where Active == 1:"));
        for (const FDoubleArray& Row : Result)
        {
            FString S; for (int32 i=0;i<Row.Values.Num();++i){ if(i) S+=TEXT(", "); S+= FString::SanitizeFloat((float)Row.Values[i]); }
            UE_LOG(LogTemp, Log, TEXT("[%s]"), *S);
        }
    }

    // Train stub network
    TArray<FDoubleArray> InputData;
    InputData.SetNum(4);
    InputData[0].Values = {30.0, 1.0};
    InputData[1].Values = {25.0, 0.0};
    InputData[2].Values = {35.0, 1.0};
    InputData[3].Values = {28.0, 0.0};

    TArray<FDoubleArray> OutputLabels;
    OutputLabels.SetNum(4);
    OutputLabels[0].Values = {1.0};
    OutputLabels[1].Values = {0.0};
    OutputLabels[2].Values = {1.0};
    OutputLabels[3].Values = {0.0};

    FActivationNetworkStub Net = TrainCategorizedConditionNeuralNetwork(InputData, OutputLabels, 300, 0.2);

    // Test the trained stub network
    FDoubleArray TestInput; TestInput.Values = {32.0, 1.0};
    const FDoubleArray Pred = ComputeActivationNetwork(Net, TestInput);
    UE_LOG(LogTemp, Log, TEXT("Prediction for Age=32, Active=1: %f"), Pred.Values.Num() > 0 ? (float)Pred.Values[0] : 0.0f);
}

bool UCategorizedDataRetrievalLibrary::Evaluate(double LHS, ENumericCompareOp Op, double RHS)
{
    switch (Op)
    {
    case ENumericCompareOp::Greater:      return LHS >  RHS;
    case ENumericCompareOp::GreaterEqual: return LHS >= RHS;
    case ENumericCompareOp::Less:         return LHS <  RHS;
    case ENumericCompareOp::LessEqual:    return LHS <= RHS;
    case ENumericCompareOp::Equal:        return LHS == RHS;
    case ENumericCompareOp::NotEqual:     return LHS != RHS;
    default: return false;
    }
}

