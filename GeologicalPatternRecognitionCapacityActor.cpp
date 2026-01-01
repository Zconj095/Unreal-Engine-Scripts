#include "GeologicalPatternRecognitionCapacityActor.h"

#include "Math/UnrealMathUtility.h"

AGeologicalPatternRecognitionCapacityActor::AGeologicalPatternRecognitionCapacityActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeologicalPatternRecognitionCapacityActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (NumGeologicalPatterns <= 0.0f)
	{
		MemoryCapacity = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("NumGeologicalPatterns must be greater than zero."));
		return;
	}

	MemoryCapacity = EncodingEfficiency * NumGeologicalPatterns * FMath::Loge(NumGeologicalPatterns);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Number of Patterns (Np): %.2f, Memory Capacity (Mg): %.4f"), NumGeologicalPatterns, MemoryCapacity);
	}
}

float AGeologicalPatternRecognitionCapacityActor::GetMemoryCapacity() const
{
	return MemoryCapacity;
}
