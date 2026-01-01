#include "GameMechanicComplexityActor.h"

#include "Math/UnrealMathUtility.h"

AGameMechanicComplexityActor::AGameMechanicComplexityActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameMechanicComplexityActor::BeginPlay()
{
	Super::BeginPlay();
	RecalculateGMC();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GMC: %.4f"), GMC);
	}
}

void AGameMechanicComplexityActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RecalculateGMC();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GMC: %.4f"), GMC);
	}
}

void AGameMechanicComplexityActor::UpdateMechanicWeights(const TArray<float>& NewWeights)
{
	MechanicWeights = NewWeights;
	RecalculateGMC();
}

void AGameMechanicComplexityActor::UpdateMechanicComplexities(const TArray<float>& NewComplexities)
{
	MechanicComplexities = NewComplexities;
	RecalculateGMC();
}

float AGameMechanicComplexityActor::GetGMC() const
{
	return GMC;
}

void AGameMechanicComplexityActor::RecalculateGMC()
{
	if (MechanicWeights.Num() != MechanicComplexities.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Mechanic weights and complexities must have the same length."));
		GMC = 0.0f;
		return;
	}

	if (MechanicWeights.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No game mechanics available for calculation."));
		GMC = 0.0f;
		return;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < MechanicWeights.Num(); ++Index)
	{
		const float ComplexityValue = MechanicComplexities[Index];
		if (ComplexityValue <= KINDA_SMALL_NUMBER)
		{
			UE_LOG(LogTemp, Warning, TEXT("Complexity at index %d must be greater than zero. Skipping."), Index);
			continue;
		}

		Sum += MechanicWeights[Index] * FMath::Log2(ComplexityValue);
	}

	GMC = Beta * Sum;
}
