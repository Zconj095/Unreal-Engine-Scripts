#include "MultidisciplinaryCoordinationIndexActor.h"
#include "Math/UnrealMathUtility.h"

AMultidisciplinaryCoordinationIndexActor::AMultidisciplinaryCoordinationIndexActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMultidisciplinaryCoordinationIndexActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	ComputeMCI();
}

void AMultidisciplinaryCoordinationIndexActor::ComputeMCI() const
{
	if (DisciplineWeights.Num() != DisciplineDemands.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Weights and demands must match length."));
		return;
	}

	if (DisciplineWeights.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No disciplines available for calculation."));
		return;
	}

	float Sum = 0.f;
	for (int32 Index = 0; Index < DisciplineWeights.Num(); ++Index)
	{
		Sum += DisciplineWeights[Index] * DisciplineDemands[Index];
	}

	const float MCI = Xi * (Sum / DisciplineWeights.Num());
	UE_LOG(LogTemp, Log, TEXT("[%.2f] MCI: %.4f"), ElapsedTime, MCI);
}
