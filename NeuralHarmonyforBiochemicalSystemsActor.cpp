#include "NeuralHarmonyforBiochemicalSystemsActor.h"
#include "Engine/Engine.h"

ANeuralHarmonyforBiochemicalSystemsActor::ANeuralHarmonyforBiochemicalSystemsActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralHarmonyforBiochemicalSystemsActor::BeginPlay()
{
    Super::BeginPlay();

    if (!AreParametersValid())
    {
        UE_LOG(LogTemp, Error, TEXT("NHBS parameters invalid on BeginPlay."));
        return;
    }

    NHBSResult = CalculateNHBS();
    if (bLogToScreen && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Initial NHBS: %.4f"), NHBSResult));
    }
    UE_LOG(LogTemp, Log, TEXT("Initial NHBS: %.4f"), NHBSResult);
}

void ANeuralHarmonyforBiochemicalSystemsActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!AreParametersValid())
    {
        return;
    }

    NHBSResult = CalculateNHBS();
    if (bLogToScreen && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("NHBS Result: %.4f"), NHBSResult));
    }
    UE_LOG(LogTemp, Log, TEXT("NHBS Result: %.4f"), NHBSResult);
}

float ANeuralHarmonyforBiochemicalSystemsActor::CalculateNHBS() const
{
    float Sum = 0.f;
    for (int32 Index = 0; Index < Phi.Num(); ++Index)
    {
        Sum += Phi[Index] * Fk[Index];
    }

    return Sum / FMath::Sqrt(FMath::Max(1.0f, static_cast<float>(ClusterCount)));
}

bool ANeuralHarmonyforBiochemicalSystemsActor::AreParametersValid() const
{
    if (Phi.Num() != Fk.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Phi and Fk arrays must match in size."));
        return false;
    }

    if (ClusterCount <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Cluster count (m) must be greater than zero."));
        return false;
    }

    return true;
}
