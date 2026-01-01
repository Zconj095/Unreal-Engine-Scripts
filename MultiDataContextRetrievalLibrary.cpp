#include "MultiDataContextRetrievalLibrary.h"

TArray<double> UMultiDataContextRetrievalLibrary::RetrieveFromMultipleStructuresAndContexts(const TArray<FNamedMatrixList>& DataStructures, const TArray<FName>& ContextFilter, ENumericCompareOp Op, double CompareValue)
{
    TArray<double> Results;
    const bool bUseFilter = ContextFilter.Num() > 0;

    for (const FNamedMatrixList& Entry : DataStructures)
    {
        if (bUseFilter && !ContextFilter.Contains(Entry.Context))
        {
            continue;
        }

        for (const FDoubleMatrix& Structure : Entry.Structures)
        {
            for (const FDoubleArray& Row : Structure.Rows)
            {
                for (double Value : Row.Values)
                {
                    if (Evaluate(Value, Op, CompareValue))
                    {
                        Results.Add(Value);
                    }
                }
            }
        }
    }

    return Results;
}

TArray<double> UMultiDataContextRetrievalLibrary::RetrieveElementsByContextConditions(const TArray<FNamedMatrixList>& DataStructures, const TArray<FContextElementCondition>& Conditions)
{
    TArray<double> Results;

    for (const FNamedMatrixList& Entry : DataStructures)
    {
        // Find matching conditions for this context
        TArray<const FContextElementCondition*> Matching;
        for (const FContextElementCondition& C : Conditions)
        {
            if (C.Context == Entry.Context)
            {
                Matching.Add(&C);
            }
        }
        if (Matching.Num() == 0)
        {
            continue;
        }

        for (const FDoubleMatrix& Structure : Entry.Structures)
        {
            for (const FDoubleArray& Row : Structure.Rows)
            {
                for (double V : Row.Values)
                {
                    for (const FContextElementCondition* C : Matching)
                    {
                        if (Evaluate(V, C->Op, C->CompareValue))
                        {
                            Results.Add(V);
                            break; // OR across conditions
                        }
                    }
                }
            }
        }
    }

    return Results;
}

FActivationNetworkStub UMultiDataContextRetrievalLibrary::TrainMultiContextConditionNeuralNetwork(const TArray<FDoubleArray>& InputData, const TArray<FDoubleArray>& OutputLabels, int32 Epochs, double LearningRate)
{
    FActivationNetworkStub Net;
    if (InputData.Num() == 0 || OutputLabels.Num() == 0 || InputData.Num() != OutputLabels.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("[MultiDataContextRetrieval] Invalid training data sizes."));
        return Net;
    }

    const int32 N = InputData.Num();
    const int32 InputSize = InputData[0].Values.Num();
    const int32 OutputSize = OutputLabels[0].Values.Num();

    Net.InputSize = InputSize;
    Net.OutputSize = OutputSize;
    Net.Weights.SetNum(OutputSize);
    Net.Biases.SetNumZeroed(OutputSize);
    for (int32 o = 0; o < OutputSize; ++o)
    {
        Net.Weights[o].Values.SetNum(InputSize);
        for (int32 i = 0; i < InputSize; ++i)
        {
            Net.Weights[o].Values[i] = FMath::FRandRange(-0.01, 0.01);
        }
        Net.Biases[o] = 0.0;
    }

    for (int32 epoch = 0; epoch < Epochs; ++epoch)
    {
        double TotalError = 0.0;
        TArray<FDoubleArray> dW; dW.SetNum(OutputSize);
        TArray<double> dB; dB.SetNumZeroed(OutputSize);
        for (int32 o = 0; o < OutputSize; ++o) { dW[o].Values.Init(0.0, InputSize); }

        for (int32 n = 0; n < N; ++n)
        {
            const TArray<double>& X = InputData[n].Values;
            const TArray<double>& Y = OutputLabels[n].Values;

            TArray<double> A; A.SetNum(OutputSize);
            for (int32 o = 0; o < OutputSize; ++o)
            {
                double z = Net.Biases[o];
                for (int32 i = 0; i < InputSize; ++i) z += Net.Weights[o].Values[i] * X[i];
                A[o] = Sigmoid(z);
            }

            for (int32 o = 0; o < OutputSize; ++o)
            {
                const double diff = A[o] - Y[o];
                const double dz = diff * A[o] * (1.0 - A[o]);
                TotalError += 0.5 * diff * diff;
                for (int32 i = 0; i < InputSize; ++i) dW[o].Values[i] += dz * X[i];
                dB[o] += dz;
            }
        }

        const double InvN = 1.0 / static_cast<double>(N);
        for (int32 o = 0; o < OutputSize; ++o)
        {
            for (int32 i = 0; i < InputSize; ++i) Net.Weights[o].Values[i] -= LearningRate * (dW[o].Values[i] * InvN);
            Net.Biases[o] -= LearningRate * (dB[o] * InvN);
        }

        if ((epoch % 50) == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Epoch %d: Error = %f"), epoch, (float)(TotalError * InvN));
        }

        if (TotalError * InvN < 0.001) break;
    }

    return Net;
}

FDoubleArray UMultiDataContextRetrievalLibrary::ComputeActivationNetwork(const FActivationNetworkStub& Network, const FDoubleArray& Input)
{
    FDoubleArray Out; Out.Values.SetNum(Network.OutputSize);
    if (Input.Values.Num() != Network.InputSize)
    {
        UE_LOG(LogTemp, Error, TEXT("[MultiDataContextRetrieval] Input size mismatch: expected %d, got %d"), Network.InputSize, Input.Values.Num());
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

void UMultiDataContextRetrievalLibrary::ExampleUsage()
{
    // Build example contexts with structures (matrices)
    FDoubleMatrix M1; M1.Rows.SetNum(2); M1.Rows[0].Values = {1.0, 30.0, 1.0}; M1.Rows[1].Values = {2.0, 25.0, 0.0};
    FDoubleMatrix M2; M2.Rows.SetNum(2); M2.Rows[0].Values = {3.0, 35.0, 1.0}; M2.Rows[1].Values = {4.0, 28.0, 0.0};

    FNamedMatrixList Morning; Morning.Context = TEXT("Morning"); Morning.Structures = { M1 };
    FNamedMatrixList Afternoon; Afternoon.Context = TEXT("Afternoon"); Afternoon.Structures = { M2 };

    TArray<FNamedMatrixList> Data; Data.Add(Morning); Data.Add(Afternoon);

    // Retrieve all scalar elements greater than 30 across both contexts
    const TArray<double> Results = RetrieveFromMultipleStructuresAndContexts(Data, /*ContextFilter*/{}, ENumericCompareOp::Greater, 30.0);
    UE_LOG(LogTemp, Log, TEXT("Retrieved Data (elements > 30):"));
    for (double V : Results)
    {
        UE_LOG(LogTemp, Log, TEXT("%f"), (float)V);
    }

    // Train stub network on example features
    TArray<FDoubleArray> InputData; InputData.SetNum(4);
    InputData[0].Values = {30.0, 1.0, 0.0};
    InputData[1].Values = {25.0, 0.0, 1.0};
    InputData[2].Values = {35.0, 1.0, 1.0};
    InputData[3].Values = {28.0, 0.0, 0.0};

    TArray<FDoubleArray> OutputLabels; OutputLabels.SetNum(4);
    OutputLabels[0].Values = {1.0};
    OutputLabels[1].Values = {0.0};
    OutputLabels[2].Values = {1.0};
    OutputLabels[3].Values = {0.0};

    FActivationNetworkStub Net = TrainMultiContextConditionNeuralNetwork(InputData, OutputLabels, 300, 0.2);
    FDoubleArray TestInput; TestInput.Values = {32.0, 1.0, 1.0};
    const FDoubleArray Pred = ComputeActivationNetwork(Net, TestInput);
    UE_LOG(LogTemp, Log, TEXT("Prediction: %f"), Pred.Values.Num() > 0 ? (float)Pred.Values[0] : 0.0f);
}

bool UMultiDataContextRetrievalLibrary::Evaluate(double LHS, ENumericCompareOp Op, double RHS)
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
