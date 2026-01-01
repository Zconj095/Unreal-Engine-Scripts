#include "RecursiveGradientComponent.h"

#include "Math/UnrealMathUtility.h"

URecursiveGradientComponent::URecursiveGradientComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URecursiveGradientComponent::BeginPlay()
{
    Super::BeginPlay();
    const float Result = ComputeRecursiveGradient(InitialX, Input, Theta, DeltaTheta, TimeSteps);
    UE_LOG(LogTemp, Log, TEXT("Recursive gradient result: %f"), Result);
}

float URecursiveGradientComponent::ComputeRecursiveGradient(float InInitialX, float InInput, float InTheta, float InDeltaTheta, int32 InTimeSteps)
{
    float Dx = 0.0f;
    float State = InInitialX;

    for (int32 Step = 0; Step < InTimeSteps; ++Step)
    {
        const float GradTheta = ComputeGradientTheta(State, InInput, InTheta);
        const float GradX = ComputeGradientX(State, InInput, InTheta);
        Dx = GradTheta * InDeltaTheta + GradX * Dx;
        State = ExampleFunction(State, InInput, InTheta);
    }

    return Dx;
}

float URecursiveGradientComponent::ExampleFunction(float X, float U, float InTheta) const
{
    return InTheta * FMath::Square(X) + U;
}

float URecursiveGradientComponent::ComputeGradientTheta(float X, float U, float InTheta) const
{
    constexpr float Epsilon = 1e-5f;
    const float F1 = ExampleFunction(X, U, InTheta + Epsilon);
    const float F2 = ExampleFunction(X, U, InTheta);
    return (F1 - F2) / Epsilon;
}

float URecursiveGradientComponent::ComputeGradientX(float X, float U, float InTheta) const
{
    constexpr float Epsilon = 1e-5f;
    const float F1 = ExampleFunction(X + Epsilon, U, InTheta);
    const float F2 = ExampleFunction(X, U, InTheta);
    return (F1 - F2) / Epsilon;
}
