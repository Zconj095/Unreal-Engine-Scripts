#include "PsychologicalSymmetryComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPsychologicalSymmetryComponent::UPsychologicalSymmetryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPsychologicalSymmetryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IntegrationSteps <= 0 || EndX <= StartX)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration parameters."));
		return;
	}

	float Dx = (EndX - StartX) / IntegrationSteps;
	float Sum = 0.0f;

	for (int32 i = 0; i < IntegrationSteps; ++i)
	{
		float X = StartX + i * Dx;
		float Wave = EvaluateWaveFunction(X, TimeParameter);
		float Conjugate = Wave;
		float SymmetryApplied = Wave;
		Sum += Conjugate * SymmetryApplied * Dx;
	}

	SymmetryValue = Sum;
	UE_LOG(LogTemp, Log, TEXT("Psychological Symmetry (S_p): %0.4f"), SymmetryValue);
}

float UPsychologicalSymmetryComponent::EvaluateWaveFunction(float X, float Time) const
{
	float Phase = X - Time;
	return FMath::Cos(Phase);
}
