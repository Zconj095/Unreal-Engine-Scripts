#include "NeuralBiosynthesisActor.h"

ANeuralBiosynthesisActor::ANeuralBiosynthesisActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralBiosynthesisActor::BeginPlay()
{
    Super::BeginPlay();

    CurrentTime = 0.f;
    Complexity = CalculateNBC(CurrentTime);
    UE_LOG(LogTemp, Log, TEXT("Initial Neural Biosynthesis Complexity: %.4f"), Complexity);
}

void ANeuralBiosynthesisActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CurrentTime += DeltaSeconds;
    Complexity = CalculateNBC(CurrentTime);
    UE_LOG(LogTemp, Log, TEXT("At time %.2fs, Neural Biosynthesis Complexity: %.4f"), CurrentTime, Complexity);
}

float ANeuralBiosynthesisActor::CalculateNBC(float Time) const
{
    return InitialComplexity * FMath::Exp(GrowthRate * Time);
}
