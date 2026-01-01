#include "MermaidEmpressSynapticPlasticityActor.h"
#include "Math/UnrealMathUtility.h"

AMermaidEmpressSynapticPlasticityActor::AMermaidEmpressSynapticPlasticityActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMermaidEmpressSynapticPlasticityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;
	const float Adaptability = CalculateAdaptability(TimeElapsed);
	UE_LOG(LogTemp, Log, TEXT("Synaptic Adaptability (Sa) at time %.2f: %.4f"), TimeElapsed, Adaptability);
}

float AMermaidEmpressSynapticPlasticityActor::CalculateAdaptability(float TimeSeconds) const
{
	return Baseline * (1.f + Sensitivity * ForceScale * FMath::Cos(2.f * PI * Frequency * TimeSeconds));
}
