#include "SDEMimicComponent.h"

#include "Math/UnrealMathUtility.h"

USDEMimicComponent::USDEMimicComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USDEMimicComponent::BeginPlay()
{
    Super::BeginPlay();

    const float Result = SimulateSDE(1.0f, 1000);
    UE_LOG(LogTemp, Log, TEXT("Result of SDE: %f"), Result);
}

float USDEMimicComponent::SimulateSDE(float InitValue, int32 Timesteps) const
{
    if (Timesteps <= 0)
    {
        return InitValue;
    }

    float State = InitValue;
    const float Dt = TotalTime / static_cast<float>(Timesteps);

    for (int32 i = 0; i < Timesteps; ++i)
    {
        const float DWeiner = FMath::FRandRange(-1.0f, 1.0f); // Unity Random.Range(-1f,1f)
        State += Mu * State * Dt + Sigma * State * DWeiner * FMath::Sqrt(Dt);
    }

    return State;
}

