#include "ContextualDataRetrievalLibrary.h"

TArray<FDoubleArray> UContextualDataRetrievalLibrary::RetrieveFromMultipleContexts(const TArray<FNamedDoubleMatrix>& ContextualSources, const TArray<FContextCondition>& Conditions)
{
    TArray<FDoubleArray> Results;

    for (const FNamedDoubleMatrix& NamedMatrix : ContextualSources)
    {
        // Find all conditions matching this context (allow multiple; row passes if any condition passes)
        TArray<const FContextCondition*> MatchingConditions;
        for (const FContextCondition& Cond : Conditions)
        {
            if (Cond.Context == NamedMatrix.Context)
            {
                MatchingConditions.Add(&Cond);
            }
        }

        if (MatchingConditions.Num() == 0)
        {
            continue; // no condition for this context
        }

        for (const FDoubleArray& Row : NamedMatrix.Matrix.Rows)
        {
            for (const FContextCondition* Cond : MatchingConditions)
            {
                if (!Row.Values.IsValidIndex(Cond->ColumnIndex))
                {
                    continue;
                }
                if (Evaluate(Row.Values[Cond->ColumnIndex], Cond->Op, Cond->CompareValue))
                {
                    Results.Add(Row);
                    break; // OR across conditions for this context
                }
            }
        }
    }

    return Results;
}

FActivationNetworkStub UContextualDataRetrievalLibrary::TrainContextConditionNeuralNetwork(const TArray<FDoubleArray>& InputData, const TArray<FDoubleArray>& OutputLabels, int32 Epochs, double LearningRate)
{
    FActivationNetworkStub Net;
    if (InputData.Num() == 0 || OutputLabels.Num() == 0 || InputData.Num() != OutputLabels.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("[ContextualDataRetrieval] Invalid training data sizes."));
        return Net;
    }

    const int32 N = InputData.Num();
    const int32 InputSize = InputData[0].Values.Num();
    const int32 OutputSize = OutputLabels[0].Values.Num();

    // Initialize network (single logistic layer stub)
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

    // Batch gradient descent
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

FDoubleArray UContextualDataRetrievalLibrary::ComputeActivationNetwork(const FActivationNetworkStub& Network, const FDoubleArray& Input)
{
    FDoubleArray Out; Out.Values.SetNum(Network.OutputSize);
    if (Input.Values.Num() != Network.InputSize)
    {
        UE_LOG(LogTemp, Error, TEXT("[ContextualDataRetrieval] Input size mismatch: expected %d, got %d"), Network.InputSize, Input.Values.Num());
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

void UContextualDataRetrievalLibrary::ExampleUsage()
{
    // Build example contextual sources
    FNamedDoubleMatrix Morning; Morning.Context = TEXT("Morning");
    Morning.Matrix.Rows.SetNum(2);
    Morning.Matrix.Rows[0].Values = { 1.0, 30.0, 1.0 };
    Morning.Matrix.Rows[1].Values = { 2.0, 25.0, 0.0 };

    FNamedDoubleMatrix Afternoon; Afternoon.Context = TEXT("Afternoon");
    Afternoon.Matrix.Rows.SetNum(2);
    Afternoon.Matrix.Rows[0].Values = { 3.0, 35.0, 1.0 };
    Afternoon.Matrix.Rows[1].Values = { 4.0, 28.0, 0.0 };

    TArray<FNamedDoubleMatrix> Sources; Sources.Add(Morning); Sources.Add(Afternoon);

    // Conditions: Age column (1) thresholds per context
    FContextCondition C1; C1.Context = TEXT("Morning");   C1.ColumnIndex = 1; C1.Op = ENumericCompareOp::Greater; C1.CompareValue = 30.0;
    FContextCondition C2; C2.Context = TEXT("Afternoon"); C2.ColumnIndex = 1; C2.Op = ENumericCompareOp::Greater; C2.CompareValue = 28.0;
    TArray<FContextCondition> Conditions; Conditions.Add(C1); Conditions.Add(C2);

    // Retrieve
    const TArray<FDoubleArray> Result = RetrieveFromMultipleContexts(Sources, Conditions);
    UE_LOG(LogTemp, Log, TEXT("Contextual Data Retrieval Results:"));
    for (const FDoubleArray& Row : Result)
    {
        FString S; for (int32 i=0;i<Row.Values.Num();++i){ if(i) S+=TEXT(", "); S+= FString::SanitizeFloat((float)Row.Values[i]); }
        UE_LOG(LogTemp, Log, TEXT("[%s]"), *S);
    }

    // Neural network stub training example (Age, Active, ContextOneHot)
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

    FActivationNetworkStub Net = TrainContextConditionNeuralNetwork(InputData, OutputLabels, 300, 0.2);

    // Test
    FDoubleArray TestInput; TestInput.Values = {32.0, 1.0, 1.0};
    const FDoubleArray Pred = ComputeActivationNetwork(Net, TestInput);
    UE_LOG(LogTemp, Log, TEXT("Prediction for Age=32, Active=1, Afternoon: %f"), Pred.Values.Num() > 0 ? (float)Pred.Values[0] : 0.0f);
}

bool UContextualDataRetrievalLibrary::Evaluate(double LHS, ENumericCompareOp Op, double RHS)
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

