#include "GameplaySystemDesignHarmonyActor.h"

#include "Math/UnrealMathUtility.h"

AGameplaySystemDesignHarmonyActor::AGameplaySystemDesignHarmonyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameplaySystemDesignHarmonyActor::BeginPlay()
{
	Super::BeginPlay();
	RecalculateGSDH();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GSDH: %.4f"), GSDHValue);
	}
}

void AGameplaySystemDesignHarmonyActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RecalculateGSDH();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GSDH: %.4f"), GSDHValue);
	}
}

void AGameplaySystemDesignHarmonyActor::UpdateMechanicImpacts(const TArray<float>& NewImpacts)
{
	MechanicImpacts = NewImpacts;
	RecalculateGSDH();
}

void AGameplaySystemDesignHarmonyActor::UpdatePlayerEngagements(const TArray<float>& NewEngagements)
{
	PlayerEngagements = NewEngagements;
	RecalculateGSDH();
}

float AGameplaySystemDesignHarmonyActor::GetGSDH() const
{
	return GSDHValue;
}

void AGameplaySystemDesignHarmonyActor::RecalculateGSDH()
{
	if (MechanicImpacts.Num() != PlayerEngagements.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Mechanics and engagements arrays must match in length."));
		GSDHValue = 0.0f;
		return;
	}

	if (MechanicImpacts.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No gameplay mechanics configured for GSDH."));
		GSDHValue = 0.0f;
		return;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < MechanicImpacts.Num(); ++Index)
	{
		Sum += MechanicImpacts[Index] * PlayerEngagements[Index];
	}

	GSDHValue = Sum / static_cast<float>(MechanicImpacts.Num());
}
