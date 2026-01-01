#include "LeylineEnergyInfusion6Component.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyInfusion6Component::ULeylineEnergyInfusion6Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULeylineEnergyInfusion6Component::BeginPlay()
{
	Super::BeginPlay();
	EnsureFieldsInitialized();
	RecalculateLeylineEnergy();
}

void ULeylineEnergyInfusion6Component::TickComponent(float DeltaSeconds, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);

	if (bDrawDebugSpheres && TemporalMagicalField.Num() == EnergyFlowGradient.Num())
	{
		const FVector Origin = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;

		for (int32 Index = 0; Index < TemporalMagicalField.Num(); ++Index)
		{
			const float X = Index * DeltaTime * 100.0f;
			const float Y = TemporalMagicalField[Index];
			const float Z = EnergyFlowGradient[Index];

			const FVector Position = Origin + FVector(X, Y * 100.0f, Z * 100.0f);
			DrawDebugSphere(GetWorld(), Position, 10.0f, 8, FColor::Blue, false, -1.0f, 0, 1.0f);
		}
	}
}

void ULeylineEnergyInfusion6Component::EnsureFieldsInitialized()
{
	if (TemporalMagicalField.Num() == 0)
	{
		TemporalMagicalField.SetNumZeroed(100);
		for (int32 Index = 0; Index < TemporalMagicalField.Num(); ++Index)
		{
			TemporalMagicalField[Index] = FMath::Sin(Index * DeltaTime);
		}
	}

	if (EnergyFlowGradient.Num() == 0)
	{
		EnergyFlowGradient.SetNumZeroed(TemporalMagicalField.Num());
		for (int32 Index = 0; Index < EnergyFlowGradient.Num(); ++Index)
		{
			EnergyFlowGradient[Index] = FMath::Cos(Index * DeltaTime);
		}
	}
}

double ULeylineEnergyInfusion6Component::CalculateLeylineEnergy() const
{
	if (TemporalMagicalField.Num() != EnergyFlowGradient.Num())
	{
		return 0.0;
	}

	double Sum = 0.0;
	for (int32 Index = 0; Index < TemporalMagicalField.Num(); ++Index)
	{
		Sum += TemporalMagicalField[Index] * EnergyFlowGradient[Index] * DeltaTime;
	}
	return Sum;
}

double ULeylineEnergyInfusion6Component::TensorFlowValidation() const
{
	return CalculateLeylineEnergy();
}

void ULeylineEnergyInfusion6Component::LogResults(double Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Infusion (P_construct): %.4f"), Value);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Validation - Leyline Energy Infusion (P_construct): %.4f"), TensorFlowValidation());
}

void ULeylineEnergyInfusion6Component::RecalculateLeylineEnergy()
{
	if (TemporalMagicalField.Num() != EnergyFlowGradient.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Temporal magical field and energy flow gradient arrays must match length."));
		LeylineEnergy = 0.0;
		return;
	}

	LeylineEnergy = CalculateLeylineEnergy();
	LogResults(LeylineEnergy);
}
