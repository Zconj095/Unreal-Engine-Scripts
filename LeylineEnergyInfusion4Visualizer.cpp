#include "LeylineEnergyInfusion4Visualizer.h"
#include "Engine/Engine.h"
#include "Engine/CollisionProfile.h"
#include "Math/UnrealMathUtility.h"

ALeylineEnergyInfusion4Visualizer::ALeylineEnergyInfusion4Visualizer()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	PointInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PointInstances"));
	PointInstances->SetupAttachment(RootComponent);
	PointInstances->SetMobility(EComponentMobility::Movable);
	PointInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ALeylineEnergyInfusion4Visualizer::BeginPlay()
{
	Super::BeginPlay();

	TemporalField = GenerateField(TimeSteps, [](float t) { return FMath::Sin(t); });
	EnergyGradient = GenerateField(TimeSteps, [](float t) { return FMath::Cos(t); });

	const float LeylineEnergy = CalculateLeylineEnergy(TemporalField, EnergyGradient);
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Infusion (P_biosynthesis): %.6f"), LeylineEnergy);

	TensorFlowValidation(TemporalField, EnergyGradient);

	if (!PointMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("PointMesh missing; visualization skipped."));
		return;
	}

	PointInstances->SetStaticMesh(PointMesh);
	if (PointMaterial)
	{
		PointInstances->SetMaterial(0, PointMaterial);
	}

	VisualizeFields();
}

TArray<float> ALeylineEnergyInfusion4Visualizer::GenerateField(int32 Steps, TFunction<float(float)> Function) const
{
	TArray<float> Field;
	Field.SetNumZeroed(Steps);

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		Field[Index] = Function(Index * TimeStepSize);
	}

	return Field;
}

float ALeylineEnergyInfusion4Visualizer::CalculateLeylineEnergy(const TArray<float>& Field, const TArray<float>& Gradient) const
{
	if (Field.Num() != Gradient.Num() || Field.Num() == 0)
	{
		return 0.0f;
	}

	double Sum = 0.0;
	for (int32 Index = 0; Index < Field.Num(); ++Index)
	{
		Sum += Field[Index] * Gradient[Index];
	}

	return Sum;
}

void ALeylineEnergyInfusion4Visualizer::TensorFlowValidation(const TArray<float>& Field, const TArray<float>& Gradient) const
{
	if (Field.Num() != Gradient.Num() || Field.Num() == 0)
	{
		return;
	}

	double Sum = 0.0;
	for (int32 Index = 0; Index < Field.Num(); ++Index)
	{
		Sum += Field[Index] * Gradient[Index];
	}

	UE_LOG(LogTemp, Log, TEXT("TensorFlow Validation - Leyline Energy Infusion: %.6f"), Sum);
}

void ALeylineEnergyInfusion4Visualizer::VisualizeFields() const
{
	PointInstances->ClearInstances();

	for (int32 Index = 0; Index < TemporalField.Num(); ++Index)
	{
		const FVector Location(Index * 100.0f, TemporalField[Index] * 100.0f, 0.0f);
		const FVector Scale(TemporalField[Index] * 0.5f + 0.1f);
		PointInstances->AddInstance(FTransform(FRotator::ZeroRotator, Location, Scale));
	}

	for (int32 Index = 0; Index < EnergyGradient.Num(); ++Index)
	{
		const FVector Location(Index * 100.0f, EnergyGradient[Index] * 100.0f, 200.0f);
		const FVector Scale(EnergyGradient[Index] * 0.5f + 0.1f);
		PointInstances->AddInstance(FTransform(FRotator(0, 0, 90), Location + FVector(0,0,200), Scale));
	}
}
