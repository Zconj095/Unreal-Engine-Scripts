#include "MolecularNetworkIntegrationActor.h"
#include "Math/UnrealMathUtility.h"

AMolecularNetworkIntegrationActor::AMolecularNetworkIntegrationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMolecularNetworkIntegrationActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeIntegration();
}

void AMolecularNetworkIntegrationActor::ComputeIntegration() const
{
	if (Weights.Num() != Inputs.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Weights and inputs must have same length."));
		return;
	}

	float Sum = 0.f;
	for (int32 Index = 0; Index < Weights.Num(); ++Index)
	{
		Sum += Weights[Index] * FMath::Loge(1.f + Inputs[Index]);
	}

	UE_LOG(LogTemp, Log, TEXT("Molecular Network Integration (I_m): %.4f"), Sum);
}
