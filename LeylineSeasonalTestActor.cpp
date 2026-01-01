#include "LeylineSeasonalTestActor.h"
#include "Engine/Engine.h"

ALeylineSeasonalTestActor::ALeylineSeasonalTestActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SeasonalEnergyComponent = CreateDefaultSubobject<ULeylineSeasonalEnergyComponent>(TEXT("SeasonalEnergy"));
}

void ALeylineSeasonalTestActor::BeginPlay()
{
	Super::BeginPlay();
	if (SeasonalEnergyComponent)
	{
		SeasonalEnergyComponent->RecalculateSeasonalEnergy();
	}
}
