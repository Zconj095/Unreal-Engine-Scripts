#include "MaterialStrengthEvaluationActor.h"
#include "Math/UnrealMathUtility.h"

AMaterialStrengthEvaluationActor::AMaterialStrengthEvaluationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMaterialStrengthEvaluationActor::BeginPlay()
{
	Super::BeginPlay();

	EvaluateStrain();
}

void AMaterialStrengthEvaluationActor::EvaluateStrain() const
{
	if (YoungsModulus <= 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Young's modulus must be greater than zero."));
		return;
	}

	const float Strain = Stress / YoungsModulus;
	UE_LOG(LogTemp, Log, TEXT("Derived strain from Hooke's law: %.6f"), Strain);
}
