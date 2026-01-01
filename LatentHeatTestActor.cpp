#include "LatentHeatTestActor.h"
#include "ElementalTransitionDynamics.h"

ALatentHeatTestActor::ALatentHeatTestActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALatentHeatTestActor::BeginPlay()
{
	Super::BeginPlay();
	UElementalTransitionDynamics::Test();
}
