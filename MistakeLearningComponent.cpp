#include "MistakeLearningComponent.h"
#include "Engine/Engine.h"

UMistakeLearningComponent::UMistakeLearningComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMistakeLearningComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize actions: CurrentAction = InitialAction (mirror Unity Start)
    CurrentAction = InitialAction;

    // Initial computation
    ComputeMistakeLearning();
    AdjustBehavior();
}

void UMistakeLearningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Continuously update learning and behavior adjustments (mirror Unity Update)
    ComputeMistakeLearning();
    AdjustBehavior();
}

void UMistakeLearningComponent::ComputeMistakeLearning()
{
    const int32 Num = ErrorMagnitudes.Num();
    if (Num == 0 || CorrectActions.Num() != Num)
    {
        UE_LOG(LogTemp, Error, TEXT("[MistakeLearning] Input arrays must be non-empty and of the same length!"));
        return;
    }

    double Sum = 0.0;
    for (int32 i = 0; i < Num; ++i)
    {
        Sum += -ErrorMagnitudes[i] * (1.0 - CorrectActions[i]);
    }
    MistakeLearningValue = Sum;

    UE_LOG(LogTemp, Log, TEXT("[MistakeLearning] Learning Gained from Mistakes (L_m): %f"), (float)MistakeLearningValue);
}

void UMistakeLearningComponent::AdjustBehavior()
{
    if (CurrentAction.Num() == 0 || ActionDimension <= 0 || InitialAction.Num() != ActionDimension)
    {
        UE_LOG(LogTemp, Error, TEXT("[MistakeLearning] Action dimension mismatch or null input!"));
        return;
    }

    // Compute dummy gradient: grad E(A_t) = -2 * A_t
    TArray<double> Gradient;
    ComputeGradient(CurrentAction, Gradient);

    // Update action: A_{t+1} = A_t - eta * grad
    const int32 N = CurrentAction.Num();
    for (int32 i = 0; i < N; ++i)
    {
        CurrentAction[i] -= LearningRate * Gradient[i];
    }

    // Log updated action as comma-separated values
    FString ActionStr;
    for (int32 i = 0; i < N; ++i)
    {
        if (i > 0) ActionStr += TEXT(", ");
        ActionStr += FString::SanitizeFloat((float)CurrentAction[i]);
    }
    UE_LOG(LogTemp, Log, TEXT("[MistakeLearning] Updated Action (A_{t+1}): %s"), *ActionStr);
}

void UMistakeLearningComponent::ComputeGradient(const TArray<double>& Action, TArray<double>& OutGradient) const
{
    const int32 N = Action.Num();
    OutGradient.SetNumUninitialized(N);
    for (int32 i = 0; i < N; ++i)
    {
        OutGradient[i] = -2.0 * Action[i];
    }
}

