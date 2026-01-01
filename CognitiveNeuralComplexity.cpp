#include "CognitiveNeuralComplexity.h"

DEFINE_LOG_CATEGORY(LogCognitiveNeuralComplexity);

UCognitiveNeuralComplexity::UCognitiveNeuralComplexity()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveNeuralComplexity::BeginPlay()
{
    Super::BeginPlay();

    PsychologicalComplexity = CalculateComplexity();
    UE_LOG(LogCognitiveNeuralComplexity, Log, TEXT("Cognitive Neural Complexity C_p(%f) = %f"), TimeAtEvaluation, PsychologicalComplexity);
}

float UCognitiveNeuralComplexity::CalculateComplexity() const
{
    return InitialComplexity * FMath::Exp(GrowthRate * TimeAtEvaluation);
}
