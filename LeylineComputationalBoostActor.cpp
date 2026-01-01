#include "LeylineComputationalBoostActor.h"
#include "LeylineComputationalBoostUtility.h"

ALeylineComputationalBoostActor::ALeylineComputationalBoostActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeylineComputationalBoostActor::BeginPlay()
{
	Super::BeginPlay();
	ULeylineComputationalBoostUtility::Test();
}
