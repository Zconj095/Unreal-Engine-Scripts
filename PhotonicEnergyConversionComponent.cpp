#include "PhotonicEnergyConversionComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPhotonicEnergyConversionComponent::UPhotonicEnergyConversionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPhotonicEnergyConversionComponent::BeginPlay()
{
	Super::BeginPlay();
	EnergyAbsorbed = 0.0f;
	ElapsedTime = 0.0f;
}

void UPhotonicEnergyConversionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ElapsedTime > TimeInterval)
	{
		return;
	}

	const float DeltaTimeScaled = DeltaTime;
	ElapsedTime += DeltaTimeScaled;
	const float PhotonFlux = CalculatePhotonFlux(ElapsedTime);
	const float IncrementalEnergy = Sigma * PhotonFlux * Efficiency * DeltaTimeScaled;
	EnergyAbsorbed += IncrementalEnergy;

	UE_LOG(LogTemp, Log, TEXT("Time: %0.2fs, Incremental Energy: %0.4f, Total Energy (E_p): %0.4f"),
		ElapsedTime, IncrementalEnergy, EnergyAbsorbed);
}

float UPhotonicEnergyConversionComponent::CalculatePhotonFlux(float Time) const
{
	if (FMath::IsNearlyZero(TimeInterval))
	{
		return 0.0f;
	}

	return FMath::Sin(2.0f * PI * Time / TimeInterval) + 1.0f;
}
