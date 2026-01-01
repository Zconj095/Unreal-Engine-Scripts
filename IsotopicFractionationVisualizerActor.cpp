#include "IsotopicFractionationVisualizerActor.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/CollisionProfile.h"
#include "Math/UnrealMathUtility.h"

FGeochemicalCycleModel::FGeochemicalCycleModel(double InStandardRatio)
	: StandardRatio(InStandardRatio)
{
}

TArray<double> FGeochemicalCycleModel::SimulateFractionation(const TArray<double>& RSamples) const
{
	TArray<double> Values;
	Values.SetNum(RSamples.Num());

	for (int32 Index = 0; Index < RSamples.Num(); ++Index)
	{
		Values[Index] = RSamples[Index] - StandardRatio;
	}

	return Values;
}

AIsotopicFractionationVisualizerActor::AIsotopicFractionationVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	PointInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PointInstances"));
	PointInstances->SetupAttachment(RootComponent);
	PointInstances->SetMobility(EComponentMobility::Movable);
	PointInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void AIsotopicFractionationVisualizerActor::BeginPlay()
{
	Super::BeginPlay();
	Model = FGeochemicalCycleModel(StandardRatio);

	if (PointMesh)
	{
		PointInstances->SetStaticMesh(PointMesh);
		if (PointMaterial)
		{
			PointInstances->SetMaterial(0, PointMaterial);
		}
	}

	TArray<double> Fractionation = Model.SimulateFractionation(SampleRatios);
	VisualizeFractionation(Fractionation);
}

void AIsotopicFractionationVisualizerActor::VisualizeFractionation(const TArray<double>& Values)
{
	if (!PointMesh || Values.Num() == 0)
	{
		return;
	}

	PointInstances->ClearInstances();

	for (int32 Index = 0; Index < Values.Num(); ++Index)
	{
		const float Height = static_cast<float>(Values[Index]) / HeightScale;
		const FVector Location(Index * 100.0f, 0.0f, Height * 100.0f);
		const FVector Scale(MarkerScale);
		const FTransform Transform(FRotator::ZeroRotator, Location, Scale);
		PointInstances->AddInstance(Transform);
	}
}
