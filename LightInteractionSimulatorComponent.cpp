#include "LightInteractionSimulatorComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

ULightInteractionSimulatorComponent::ULightInteractionSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentLightSource = InitialLightSource;
}

void ULightInteractionSimulatorComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentLightSource = InitialLightSource;
}

void ULightInteractionSimulatorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentLightSource = InitialLightSource + FVector(
		FMath::Sin(GetWorld()->GetTimeSeconds() * LightMovementSpeed) * 200.0f,
		0.0f,
		FMath::Cos(GetWorld()->GetTimeSeconds() * LightMovementSpeed) * 200.0f
	);

	if (GetWorld())
	{
		DrawDebugSphere(GetWorld(), CurrentLightSource, 10.0f, 12, FColor::Yellow);
	}

	if (Voxels.Num() == 0)
	{
		return;
	}

	SimulateLight();
}

void ULightInteractionSimulatorComponent::SimulateLight()
{
	if (!GetWorld())
	{
		return;
	}

	for (const FNanoVoxelWithPolarization& Voxel : Voxels)
	{
		if (!Voxel.bIsElectricallyActive)
		{
			continue;
		}

		DrawLightPath(Voxel);
	}
}

void ULightInteractionSimulatorComponent::DrawLightPath(const FNanoVoxelWithPolarization& Voxel)
{
	const FVector LightDir = (Voxel.Position - CurrentLightSource).GetSafeNormal();
	const float Distance = FVector::Dist(CurrentLightSource, Voxel.Position);
	const float IncidentAngle = FMath::Acos(FVector::DotProduct(-LightDir, FVector::UpVector));

	float RefractedAngleRadians = 0.0f;
	if (Voxel.OpticalRefractiveIndex > KINDA_SMALL_NUMBER)
	{
		const float SinValue = FMath::Sin(IncidentAngle) / Voxel.OpticalRefractiveIndex;
		if (FMath::Abs(SinValue) <= 1.0f)
		{
			RefractedAngleRadians = FMath::Asin(SinValue);
		}
	}

	const FVector RefractedDirection = FQuat(FVector::CrossProduct(FVector::UpVector, LightDir), RefractedAngleRadians).RotateVector(FVector::UpVector);
	const float Attenuation = FMath::Exp(-Distance * Voxel.MaterialDensity);
	const FLinearColor DrawColor = Voxel.DisplayColor * Attenuation;

	DrawDebugLine(GetWorld(), CurrentLightSource, Voxel.Position + RefractedDirection * 50.0f, DrawColor.ToFColor(true), false, 0.0f, 0, 2.0f);
}
