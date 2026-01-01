#include "DesignOptimization.h"

DEFINE_LOG_CATEGORY(LogDesignOptimization);

UDesignOptimization::UDesignOptimization()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDesignOptimization::BeginPlay()
{
    Super::BeginPlay();

    Efficiency = CalculateEfficiency();
    UE_LOG(LogDesignOptimization, Log, TEXT("Efficiency (η): %.6f"), Efficiency);
}

float UDesignOptimization::CalculateEfficiency() const
{
    if (FMath::IsNearlyZero(InputValue))
    {
        UE_LOG(LogDesignOptimization, Warning, TEXT("Input must be non-zero."));
        return 0.f;
    }

    return OutputValue / InputValue;
}
