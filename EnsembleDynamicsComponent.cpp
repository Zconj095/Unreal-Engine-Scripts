#include "EnsembleDynamicsComponent.h"

#include "Math/UnrealMathUtility.h"

UEnsembleDynamicsComponent::UEnsembleDynamicsComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // Unity's Update()
}

void UEnsembleDynamicsComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UEnsembleDynamicsComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const TArray<float> SpinDynamics = ComputeSpinDynamics();

    // Build comma-separated output like string.Join
    FString Output;
    Output.Reserve(SpinDynamics.Num() * 8);
    for (int32 i = 0; i < SpinDynamics.Num(); ++i)
    {
        if (i > 0) { Output += TEXT(", "); }
        Output += FString::SanitizeFloat(SpinDynamics[i]);
    }
    UE_LOG(LogTemp, Log, TEXT("%s"), *Output);
}

TArray<float> UEnsembleDynamicsComponent::ComputeSpinDynamics() const
{
    TArray<float> Values;
    const int32 N = FMath::Max(0, NumSpins);
    Values.SetNum(N);

    // Unity Time.time equivalent
    const float T = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
    for (int32 i = 0; i < N; ++i)
    {
        Values[i] = FMath::Cos(T + static_cast<float>(i));
    }
    return Values;
}

