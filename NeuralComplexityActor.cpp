#include "NeuralComplexityActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralComplexityActor::ANeuralComplexityActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralComplexityActor::BeginPlay()
{
    Super::BeginPlay();

    double Complexity = CalculateNeuralComplexity();
    UE_LOG(LogTemp, Log, TEXT("Neural Complexity (C): %.6e"), Complexity);
}

double ANeuralComplexityActor::CalculateNeuralComplexity() const
{
    if (NumberOfNeurons <= 0.0 || ConnectivityConstant <= 0.0 || QuantumEntanglementFactor < 0.0 || QuantumEntanglementFactor > 1.0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid parameters for Neural Complexity calculation."));
        return 0.0;
    }

    return ConnectivityConstant * NumberOfNeurons * FMath::Loge(NumberOfNeurons) * QuantumEntanglementFactor;
}
