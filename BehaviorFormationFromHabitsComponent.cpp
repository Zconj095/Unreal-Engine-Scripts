#include "BehaviorFormationFromHabitsComponent.h"

#include "Math/UnrealMathUtility.h"

UBehaviorFormationFromHabitsComponent::UBehaviorFormationFromHabitsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBehaviorFormationFromHabitsComponent::BeginPlay()
{
    Super::BeginPlay();
    // Initialize the tiny MLP with input dimension = number of habits (if >0)
    const int32 InDim = HabitStrengths.Num();
    if (InDim > 0)
    {
        MLP.Init(InDim, 10);
    }
    ComputeBehaviorStrength();
}

void UBehaviorFormationFromHabitsComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (bAutoUpdateEachTick)
    {
        ComputeBehaviorStrength();
    }
}

void UBehaviorFormationFromHabitsComponent::ComputeBehaviorStrength()
{
    if (HabitStrengths.Num() == 0 || ResponseMagnitudes.Num() == 0 || HabitStrengths.Num() != ResponseMagnitudes.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Habit strengths and response magnitudes must be non-empty and have the same length!"));
        return;
    }

    double Sum = 0.0;
    for (int32 i = 0; i < HabitStrengths.Num(); ++i)
    {
        Sum += static_cast<double>(HabitStrengths[i]) * static_cast<double>(ResponseMagnitudes[i]);
    }
    BehaviorStrength = static_cast<float>(Sum);
    UE_LOG(LogTemp, Log, TEXT("[BehaviorFormation] Behavior Strength from Habits (Bh): %f"), BehaviorStrength);
}

float UBehaviorFormationFromHabitsComponent::PredictBehavior(const TArray<float>& Inputs) const
{
    if (Inputs.Num() != MLP.InSize || MLP.InSize == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("PredictBehavior: Input size %d does not match network input size %d."), Inputs.Num(), MLP.InSize);
        return 0.0f;
    }
    return MLP.Forward(Inputs);
}

void UBehaviorFormationFromHabitsComponent::UpdateHabitsAndResponses(const TArray<float>& NewHabitStrengths, const TArray<float>& NewResponseMagnitudes)
{
    if (NewHabitStrengths.Num() != NewResponseMagnitudes.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("New habit strengths and response magnitudes must have the same length!"));
        return;
    }
    HabitStrengths = NewHabitStrengths;
    ResponseMagnitudes = NewResponseMagnitudes;

    // Re-init the MLP if input size changed
    if (MLP.InSize != HabitStrengths.Num() && HabitStrengths.Num() > 0)
    {
        MLP.Init(HabitStrengths.Num(), 10);
    }
}

// -------------------- Dynamic MLP --------------------

void UBehaviorFormationFromHabitsComponent::FDynamicMLP::Init(int32 InDim, int32 InHiddenSize)
{
    InSize = InDim;
    HiddenSize = InHiddenSize;

    W1.SetNum(HiddenSize * InSize);
    B1.SetNum(HiddenSize);
    W2.SetNum(HiddenSize);
    B2 = 0.0f;

    const float Scale = 0.1f;
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        for (int32 i = 0; i < InSize; ++i)
        {
            W1[h * InSize + i] = FMath::FRandRange(-Scale, Scale);
        }
        B1[h] = FMath::FRandRange(-Scale, Scale);
        W2[h] = FMath::FRandRange(-Scale, Scale);
    }
    B2 = FMath::FRandRange(-Scale, Scale);
}

float UBehaviorFormationFromHabitsComponent::FDynamicMLP::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float UBehaviorFormationFromHabitsComponent::FDynamicMLP::Forward(const TArray<float>& Input) const
{
    // Hidden activations
    TArray<float> Hidden;
    Hidden.SetNumZeroed(HiddenSize);
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        float Z = B1[h];
        const int32 RowOffset = h * InSize;
        for (int32 i = 0; i < InSize; ++i)
        {
            Z += W1[RowOffset + i] * Input[i];
        }
        Hidden[h] = Sigmoid(Z);
    }

    // Output
    float Zout = B2;
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        Zout += W2[h] * Hidden[h];
    }
    return Sigmoid(Zout);
}

