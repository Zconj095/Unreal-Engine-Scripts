#include "LeylineEnergyAssemblyActor.h"
#include "LeylineEnergyAssemblyUtility.h"

ALeylineEnergyAssemblyActor::ALeylineEnergyAssemblyActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeylineEnergyAssemblyActor::BeginPlay()
{
	Super::BeginPlay();
	ULeylineEnergyAssemblyUtility::Test();
}
