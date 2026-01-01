#include "LeylineEnergyIntegrationActor.h"
#include "LeylineEnergyIntegrationUtility.h"

ALeylineEnergyIntegrationActor::ALeylineEnergyIntegrationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeylineEnergyIntegrationActor::BeginPlay()
{
	Super::BeginPlay();
	ULeylineEnergyIntegrationUtility::Test();
}
