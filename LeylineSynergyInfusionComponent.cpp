#include "LeylineSynergyInfusionComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

ULeylineSynergyInfusionComponent::ULeylineSynergyInfusionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineSynergyInfusionComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateSynergyPower();
	if (bDrawGizmos)
	{
		const double Step = (EndTime - StartTime) / FMath::Max(1, Resolution);
		for (int32 Index = 0; Index <= Resolution; ++Index)
		{
			const double Time = StartTime + Index * Step;
			const double Energy = Phi(Time);
			const FVector Position(Time * 100.0, Energy * 100.0, 0.0);
			const FVector BaseLocation = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
			DrawDebugSphere(GetWorld(), Position + BaseLocation, 5.0f, 8, FColor::Cyan, false, 5.0f);
		}
	}
}

double ULeylineSynergyInfusionComponent::Phi(double Time) const
{
	return FMath::Sin(Time);
}

double ULeylineSynergyInfusionComponent::GradientMagnitude(double Time) const
{
	const FVector Gradient(FMath::Cos(Time), FMath::Sin(Time), 0.0f);
	return Gradient.Size();
}

double ULeylineSynergyInfusionComponent::CalculateSynergyPower() const
{
	if (Resolution <= 0 || EndTime <= StartTime)
	{
		return 0.0;
	}

	const double Step = (EndTime - StartTime) / Resolution;
	double Sum = 0.0;
	for (int32 Index = 0; Index <= Resolution; ++Index)
	{
		const double Time = StartTime + Index * Step;
		Sum += Phi(Time) * GradientMagnitude(Time);
	}

	return Sum * Step;
}

void ULeylineSynergyInfusionComponent::LogResult(double Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Leyline Synergy Power: %.4f"), Value);
}

void ULeylineSynergyInfusionComponent::RecalculateSynergyPower()
{
	SynergyPower = CalculateSynergyPower();
	LogResult(SynergyPower);
}
