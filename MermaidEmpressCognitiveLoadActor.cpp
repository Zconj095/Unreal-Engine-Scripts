#include "MermaidEmpressCognitiveLoadActor.h"
#include "Math/UnrealMathUtility.h"

AMermaidEmpressCognitiveLoadActor::AMermaidEmpressCognitiveLoadActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMermaidEmpressCognitiveLoadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	CognitiveLoad = CalculateCognitiveLoad();
	UE_LOG(LogTemp, Log, TEXT("Time %.2f: Cognitive Load = %.4f"), ElapsedTime, CognitiveLoad);
}

float AMermaidEmpressCognitiveLoadActor::CalculateCognitiveLoad() const
{
	if (MemoryCapacity < 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Memory capacity must be non-negative."));
		return 0.f;
	}

	return ProportionalityConstant * QuantumComplexity * FMath::LogX(10.f, MemoryCapacity + 1.f);
}
