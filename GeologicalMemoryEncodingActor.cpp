#include "GeologicalMemoryEncodingActor.h"

#include "Math/UnrealMathUtility.h"

AGeologicalMemoryEncodingActor::AGeologicalMemoryEncodingActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeologicalMemoryEncodingActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentTime += DeltaSeconds;
	const float Complexity = CalculateGeologicalComplexity(CurrentTime);
	CognitiveMemoryCapacity += EncodingEfficiency * Complexity * TimeStep;

	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Time: %.2f, Geological Complexity: %.4f, Memory Capacity: %.4f"), CurrentTime, Complexity, CognitiveMemoryCapacity);
	}
}

void AGeologicalMemoryEncodingActor::ResetMemoryCapacity(float NewTime)
{
	CognitiveMemoryCapacity = 0.0f;
	CurrentTime = NewTime;
}

float AGeologicalMemoryEncodingActor::GetMemoryCapacity() const
{
	return CognitiveMemoryCapacity;
}

float AGeologicalMemoryEncodingActor::CalculateGeologicalComplexity(float TimeValue) const
{
	return InitialComplexity * FMath::Exp(GrowthRate * TimeValue);
}
