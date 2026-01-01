#include "EvolutionVisualizerComponent.h"
#include "Kismet/GameplayStatics.h"

UEvolutionVisualizerComponent::UEvolutionVisualizerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    Lambda = 0.1;
    Traits = {5.0,3.0,2.0};
}

void UEvolutionVisualizerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!PointActorClass || !GetWorld())
    {
        return;
    }

    for(double X=1.0;X<=5.0;X+=0.5)
    {
        const double Fitness=EvaluateFitness(X);
        const FVector Location(X, Fitness, 0.0);
        AActor* Actor=GetWorld()->SpawnActor<AActor>(PointActorClass,Location,FRotator::ZeroRotator);
        if(Actor)
        {
            Actor->SetActorScale3D(FVector(0.2f));
        }
    }
}

double UEvolutionVisualizerComponent::EvaluateFitness(double X) const
{
    double BenefitSum=0.0;
    for(double Phi:Traits)
    {
        BenefitSum+=Phi*FMath::Exp(-X);
    }
    return BenefitSum-(Lambda*X*X);
}
