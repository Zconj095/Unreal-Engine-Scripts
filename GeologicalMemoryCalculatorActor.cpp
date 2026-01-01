#include "GeologicalMemoryCalculatorActor.h"

#include "Math/UnrealMathUtility.h"

AGeologicalMemoryCalculatorActor::AGeologicalMemoryCalculatorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeologicalMemoryCalculatorActor::BeginPlay()
{
	Super::BeginPlay();

	MemoryCapacity = CalculateGeologicalMemory();
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Geological Memory Capacity (M_g): %.4f"), MemoryCapacity);
	}
}

float AGeologicalMemoryCalculatorActor::GetMemoryCapacity() const
{
	return MemoryCapacity;
}

float AGeologicalMemoryCalculatorActor::GeologicalComplexity(float TimeValue) const
{
	return FMath::Sin(TimeValue) + 2.0f;
}

float AGeologicalMemoryCalculatorActor::CalculateGeologicalMemory() const
{
	if (Steps <= 0 || EndTime <= StartTime)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid time interval or steps for geological memory calculation."));
		return 0.0f;
	}

	const float DeltaT = (EndTime - StartTime) / Steps;
	float Memory = 0.0f;

	for (int32 i = 0; i < Steps; ++i)
	{
		const float CurrentTime = StartTime + i * DeltaT;
		const float NextTime = CurrentTime + DeltaT;
		const float Midpoint = 0.5f * (CurrentTime + NextTime);
		const float Complexity = GeologicalComplexity(Midpoint);
		Memory += Alpha * Complexity * DeltaT;
	}

	return Memory;
}
