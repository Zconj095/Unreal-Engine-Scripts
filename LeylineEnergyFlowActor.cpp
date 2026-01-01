#include "LeylineEnergyFlowActor.h"
#include "LeylineEnergyFlowUtility.h"

ALeylineEnergyFlowActor::ALeylineEnergyFlowActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeylineEnergyFlowActor::BeginPlay()
{
	Super::BeginPlay();

	const double EnergyFlow = ULeylineEnergyFlowUtility::ComputeLeylineEnergyFlow(BoundsMin, BoundsMax, Steps);
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Flow: %.2f"), EnergyFlow);

	ULeylineEnergyFlowUtility::Test();
}
