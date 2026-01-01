#include "LeylineMarineInfluenceComponent.h"
#include "Engine/Engine.h"

ULeylineMarineInfluenceComponent::ULeylineMarineInfluenceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineMarineInfluenceComponent::BeginPlay()
{
	Super::BeginPlay();
	RunTest();
}

double ULeylineMarineInfluenceComponent::ComputeDotProduct(const FVector& FieldStrength, const FVector& EnergyGradient) const
{
	return FVector::DotProduct(FieldStrength, EnergyGradient);
}

double ULeylineMarineInfluenceComponent::ComputeMarinePower() const
{
	const TArray<FVector> FieldStrengths = {
		FVector(2, 3, 1),
		FVector(1, 0, 4)
	};
	const TArray<FVector> EnergyGradients = {
		FVector(0.5f, 1.0f, 0.2f),
		FVector(0.3f, 0.7f, 0.9f)
	};
	const TArray<double> Volumes = { 10.0, 15.0 };

	double Power = 0.0;
	for (int32 Index = 0; Index < FieldStrengths.Num(); ++Index)
	{
		Power += ComputeDotProduct(FieldStrengths[Index], EnergyGradients[Index]) * Volumes[Index];
	}

	return Power;
}

void ULeylineMarineInfluenceComponent::RunTest()
{
	double Power = ComputeMarinePower();
	UE_LOG(LogTemp, Log, TEXT("Power Transferred to Marine Systems: %.4f units"), Power);
}
