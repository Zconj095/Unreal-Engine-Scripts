#include "EvolutionaryPathwaySimulationComponent.h"
#include "Math/UnrealMathUtility.h"

UEvolutionaryPathwaySimulationComponent::UEvolutionaryPathwaySimulationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	TraitBenefits = {5.0,3.0,2.0};
	Lambda = 0.1;
	XStart = 1.0;
	XEnd = 5.0;
	XStep = 1.0;
}

void UEvolutionaryPathwaySimulationComponent::BeginPlay()
{
	Super::BeginPlay();

	for(double x=XStart;x<=XEnd;x+=XStep)
	{
		double Fitness=ComputeFitness(x);
		UE_LOG(LogTemp,Log,TEXT("Genotype Complexity x=%f Fitness F(x)=%f"),x,Fitness);
	}
}

double UEvolutionaryPathwaySimulationComponent::ComputeFitness(double X) const
{
	double BenefitSum=0.0;
	for(double Phi:TraitBenefits)
	{
		BenefitSum+=Phi*FMath::Exp(-X);
	}
	return BenefitSum-ComplexityCost(X);
}

double UEvolutionaryPathwaySimulationComponent::ComplexityCost(double X) const
{
	return Lambda*X*X;
}
