#include "MolecularAssemblyDynamicsActor.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

AMolecularAssemblyDynamicsActor::AMolecularAssemblyDynamicsActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMolecularAssemblyDynamicsActor::BeginPlay()
{
	Super::BeginPlay();

	const float Energy = ComputeBindingEnergy(Distance);
	UE_LOG(LogTemp, Log, TEXT("Binding Energy (Eb): %.4f"), Energy);
}

void AMolecularAssemblyDynamicsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	for (float DistanceValue = 0.01f; DistanceValue <= VisualizationDuration + 0.01f; DistanceValue += 0.005f)
	{
		const float Energy = ComputeBindingEnergy(DistanceValue);
		const FVector Location(DistanceValue * 100.f, Energy, 0.f);
		DrawDebugSphere(GetWorld(), Location, 5.f, 8, FColor::Red, false, -1.f);
	}
}

float AMolecularAssemblyDynamicsActor::ComputeBindingEnergy(float DistanceValue) const
{
	const float Electrostatic = (CoulombConstant * ChargeA * ChargeB) / DistanceValue;
	return Electrostatic + MagicalGradient;
}
