#include "NeuralEvolutionaryFitnessActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEvolutionaryFitnessActor::ANeuralEvolutionaryFitnessActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralEvolutionaryFitnessActor::BeginPlay()
{
    Super::BeginPlay();

    const float Fitness = CalculateFitness(Capacity, Energy);
    UE_LOG(LogTemp, Log, TEXT("Neural Fitness (F): %.4f"), Fitness);

    const float Prediction = PredictFitness(Capacity, Energy);
    UE_LOG(LogTemp, Log, TEXT("Predicted Neural Fitness (F): %.4f"), Prediction);

    const float TFResult = ComputeWithMockTensorFlow(Capacity, Energy);
    UE_LOG(LogTemp, Log, TEXT("TensorFlow Simulation Result (F): %.4f"), TFResult);
}

float ANeuralEvolutionaryFitnessActor::PredictFitness(float CapacityValue, float EnergyValue) const
{
    return CalculateFitness(CapacityValue, EnergyValue);
}

float ANeuralEvolutionaryFitnessActor::CalculateFitness(float CapacityValue, float EnergyValue) const
{
    if (EnergyValue <= 0.f)
    {
        UE_LOG(LogTemp, Error, TEXT("Energy expenditure must be greater than zero!"));
        return FLT_MAX;
    }

    return CapacityValue / EnergyValue;
}

float ANeuralEvolutionaryFitnessActor::ComputeWithMockTensorFlow(float CapacityValue, float EnergyValue) const
{
    return CalculateFitness(CapacityValue, EnergyValue);
}
