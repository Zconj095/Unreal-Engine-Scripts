#include "GameEvolutionaryAdaptabilityIndexActor.h"

#include "Math/UnrealMathUtility.h"

AGameEvolutionaryAdaptabilityIndexActor::AGameEvolutionaryAdaptabilityIndexActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameEvolutionaryAdaptabilityIndexActor::BeginPlay()
{
	Super::BeginPlay();
	RecalculateGEAI();
	if (bLogGEAI)
	{
		UE_LOG(LogTemp, Display, TEXT("GEAI: %.4f"), GEAI);
	}
}

void AGameEvolutionaryAdaptabilityIndexActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RecalculateGEAI();
	if (bLogGEAI)
	{
		UE_LOG(LogTemp, Display, TEXT("GEAI: %.4f"), GEAI);
	}
}

void AGameEvolutionaryAdaptabilityIndexActor::UpdateEta(float NewEta)
{
	Eta = NewEta;
	RecalculateGEAI();
}

void AGameEvolutionaryAdaptabilityIndexActor::UpdateDeltaGDesign(float NewDeltaGDesign)
{
	DeltaGDesign = NewDeltaGDesign;
	RecalculateGEAI();
}

void AGameEvolutionaryAdaptabilityIndexActor::UpdateDeltaTime(float NewDeltaTime)
{
	if (NewDeltaTime > KINDA_SMALL_NUMBER)
	{
		DeltaTime = NewDeltaTime;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Time interval (Δt) must be greater than zero."));
	}
	RecalculateGEAI();
}

float AGameEvolutionaryAdaptabilityIndexActor::GetGEAI() const
{
	return GEAI;
}

void AGameEvolutionaryAdaptabilityIndexActor::RecalculateGEAI()
{
	if (DeltaTime <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Time interval (Δt) must be greater than zero."));
		GEAI = 0.0f;
		return;
	}

	GEAI = Eta * (DeltaGDesign / DeltaTime);
}
