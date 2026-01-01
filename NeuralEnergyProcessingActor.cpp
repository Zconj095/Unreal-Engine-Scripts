#include "NeuralEnergyProcessingActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEnergyProcessingActor::ANeuralEnergyProcessingActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralEnergyProcessingActor::BeginPlay()
{
    Super::BeginPlay();

    InitializeParameters();

    InputVector.SetNum(InputSize);
    for (int32 Index = 0; Index < InputSize; ++Index)
    {
        InputVector[Index] = FMath::FRandRange(-1.f, 1.f);
    }

    TArray<float> Integrated = EnergyIntegration(InputVector);
    TArray<float> Amplified = EnergyAmplification(Integrated);
    TArray<float> Controlled = ControlledOutput(Amplified);
    TArray<float> SoftmaxOutput = Softmax(Controlled);

    auto JoinValues = [](const TArray<float>& Values)
    {
        TArray<FString> Components;
        for (float Value : Values)
        {
            Components.Add(FString::SanitizeFloat(Value));
        }
        return FString::Join(Components, TEXT(", "));
    };

    UE_LOG(LogTemp, Log, TEXT("Input: [%s]"), *JoinValues(InputVector));
    UE_LOG(LogTemp, Log, TEXT("Integrated Output (y): [%s]"), *JoinValues(Integrated));
    UE_LOG(LogTemp, Log, TEXT("Amplified Output: [%s]"), *JoinValues(Amplified));
    UE_LOG(LogTemp, Log, TEXT("Controlled Output: [%s]"), *JoinValues(Controlled));
    UE_LOG(LogTemp, Log, TEXT("Softmax Output: [%s]"), *JoinValues(SoftmaxOutput));
}

void ANeuralEnergyProcessingActor::InitializeParameters()
{
    WeightMatrix.SetNum(OutputSize);
    for (int32 Row = 0; Row < OutputSize; ++Row)
    {
        WeightMatrix[Row].SetNum(InputSize);
        for (int32 Col = 0; Col < InputSize; ++Col)
        {
            WeightMatrix[Row][Col] = FMath::FRandRange(-0.5f, 0.5f);
        }
    }

    BiasVector.SetNum(OutputSize);
    for (int32 Index = 0; Index < OutputSize; ++Index)
    {
        BiasVector[Index] = FMath::FRandRange(-0.5f, 0.5f);
    }
}

TArray<float> ANeuralEnergyProcessingActor::EnergyIntegration(const TArray<float>& Input) const
{
    TArray<float> Result;
    Result.SetNum(OutputSize);
    for (int32 Row = 0; Row < OutputSize; ++Row)
    {
        float Sum = 0.f;
        for (int32 Col = 0; Col < InputSize; ++Col)
        {
            Sum += WeightMatrix[Row][Col] * Input[Col];
        }
        Result[Row] = FMath::Tan(Sum + BiasVector[Row]);
    }
    return Result;
}

TArray<float> ANeuralEnergyProcessingActor::EnergyAmplification(const TArray<float>& Input) const
{
    TArray<float> Result;
    Result.SetNum(OutputSize);
    for (int32 Index = 0; Index < OutputSize; ++Index)
    {
        float Value = Input[Index];
        Result[Index] = FMath::Sign(Value) * FMath::Square(FMath::Abs(Value));
    }
    return Result;
}

TArray<float> ANeuralEnergyProcessingActor::ControlledOutput(const TArray<float>& Input) const
{
    float MaxAbs = 0.f;
    for (float Value : Input)
    {
        MaxAbs = FMath::Max(MaxAbs, FMath::Abs(Value));
    }
    MaxAbs += Epsilon;

    TArray<float> Result;
    Result.SetNum(OutputSize);
    for (int32 Index = 0; Index < OutputSize; ++Index)
    {
        Result[Index] = Input[Index] / MaxAbs;
    }
    return Result;
}

TArray<float> ANeuralEnergyProcessingActor::Softmax(const TArray<float>& Input) const
{
    TArray<float> ExpValues;
    ExpValues.SetNum(OutputSize);
    float Sum = 0.f;
    for (int32 Index = 0; Index < OutputSize; ++Index)
    {
        ExpValues[Index] = FMath::Exp(Input[Index]);
        Sum += ExpValues[Index];
    }

    TArray<float> Result;
    Result.SetNum(OutputSize);
    if (Sum == 0.f)
    {
        return Result;
    }

    for (int32 Index = 0; Index < OutputSize; ++Index)
    {
        Result[Index] = ExpValues[Index] / Sum;
    }
    return Result;
}
