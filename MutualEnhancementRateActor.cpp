#include "MutualEnhancementRateActor.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

AMutualEnhancementRateActor::AMutualEnhancementRateActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMutualEnhancementRateActor::BeginPlay()
{
	Super::BeginPlay();

	const float EnhancementRate = ComputeEnhancementRate();
	UE_LOG(LogTemp, Log, TEXT("Mutual Enhancement Rate (R): %.4f"), EnhancementRate);
}

void AMutualEnhancementRateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Elapsed += DeltaTime;
	const float TotalEnergy = BiologicalEnergy + MagicalEnergy;
	if (TotalEnergy <= 0.f)
	{
		return;
	}

	const float BioHeight = BiologicalEnergy / TotalEnergy * 100.f;
	const float MagicHeight = MagicalEnergy / TotalEnergy * 100.f;

	DrawDebugBox(GetWorld(), FVector(-100.f, BioHeight, 0.f), FVector(25.f, BioHeight, 25.f), FColor::Green, false, -1.f);
	DrawDebugBox(GetWorld(), FVector(100.f, MagicHeight, 0.f), FVector(25.f, MagicHeight, 25.f), FColor::Magenta, false, -1.f);
}

float AMutualEnhancementRateActor::ComputeEnhancementRate() const
{
	return Alpha * BiologicalEnergy * MagicalEnergy;
}
