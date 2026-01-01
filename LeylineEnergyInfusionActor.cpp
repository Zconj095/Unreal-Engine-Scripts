#include "LeylineEnergyInfusionActor.h"
#include "LeylineEnergyInfusionUtility.h"

ALeylineEnergyInfusionActor::ALeylineEnergyInfusionActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeylineEnergyInfusionActor::BeginPlay()
{
	Super::BeginPlay();
	ULeylineEnergyInfusionUtility::Test();
}
