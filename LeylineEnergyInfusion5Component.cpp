#include "LeylineEnergyInfusion5Component.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyInfusion5Component::ULeylineEnergyInfusion5Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULeylineEnergyInfusion5Component::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLeylineEnergy();
}

void ULeylineEnergyInfusion5Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDrawDebug && TemporalMagicalField.Num() == EnergyFlowGradient.Num())
	{
		const FVector Origin = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
		for (int32 Index = 0; Index < TemporalMagicalField.Num(); ++Index)
		{
			const FVector Position = Origin + FVector(Index * TimeStep * 100.0f, TemporalMagicalField[Index] * EnergyFlowGradient[Index] * 100.0f, 0.0f);
			DrawDebugSphere(GetWorld(), Position, 10.0f, 8, FColor::Cyan, false, -1.0f, 0, 1.0f);
		}
	}
}

double ULeylineEnergyInfusion5Component::CalculateLeylineEnergy() const
{
	if (TemporalMagicalField.Num() != EnergyFlowGradient.Num())
	{
		return 0.0;
	}

	double Sum = 0.0;
	for (int32 Index = 0; Index < TemporalMagicalField.Num(); ++Index)
	{
		Sum += TemporalMagicalField[Index] * EnergyFlowGradient[Index] * TimeStep;
	}

	return Sum;
}

double ULeylineEnergyInfusion5Component::TensorFlowValidation() const
{
	if (TemporalMagicalField.Num() != EnergyFlowGradient.Num())
	{
		return 0.0;
	}

	double Sum = 0.0;
	for (int32 Index = 0; Index < TemporalMagicalField.Num(); ++Index)
	{
		Sum += TemporalMagicalField[Index] * EnergyFlowGradient[Index] * TimeStep;
	}

	return Sum;
}

void ULeylineEnergyInfusion5Component::LogLeylineEnergy(double Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Infusion (P_bio): %.4f"), Value);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Validation - Leyline Energy Infusion (P_bio): %.4f"), TensorFlowValidation());
}

void ULeylineEnergyInfusion5Component::RecalculateLeylineEnergy()
{
	if (TemporalMagicalField.Num() != EnergyFlowGradient.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Temporal magical field and energy gradient arrays must have the same length."));
		LeylineEnergy = 0.0;
		return;
	}

	LeylineEnergy = CalculateLeylineEnergy();
	LogLeylineEnergy(LeylineEnergy);
}
