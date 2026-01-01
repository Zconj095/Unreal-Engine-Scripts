#include "LeylineBiosynthesisActor.h"

ALeylineBiosynthesisActor::ALeylineBiosynthesisActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeylineBiosynthesisActor::BeginPlay()
{
	Super::BeginPlay();
	ULeylineBiosynthesisUtility::Test();
}
