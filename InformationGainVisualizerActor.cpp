#include "InformationGainVisualizerActor.h"
#include "Engine/Engine.h"
#include "Engine/CollisionProfile.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

double FPhylogeneticInformationModel::ComputeInformationGain(const TArray<double>& Current, const TArray<double>& Ancestral) const
{
	if (Current.Num() != Ancestral.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Current and ancestral probability arrays must be the same length."));
		return 0.0;
	}

	double Gain = 0.0;
	for (int32 Index = 0; Index < Current.Num(); ++Index)
	{
		const double CurrentValue = Current[Index];
		const double AncestralValue = Ancestral[Index];

		if (CurrentValue <= 0.0 || AncestralValue <= 0.0)
		{
			continue;
		}

		Gain += CurrentValue * FMath::Log2(CurrentValue / AncestralValue);
	}
	return Gain;
}

//////////////////////////////////////////////////////////////////////////
// AInformationGainVisualizerActor

AInformationGainVisualizerActor::AInformationGainVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	BarInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BarInstances"));
	BarInstances->SetupAttachment(RootComponent);
	BarInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	BarInstances->SetMobility(EComponentMobility::Movable);
}

void AInformationGainVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	EnsureProbabilitySizesMatch();

	if (!BarMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("BarMesh is not set; visualization will be skipped."));
	}
	else
	{
		BarInstances->SetStaticMesh(BarMesh);
		if (BarMaterial)
		{
			BarInstances->SetMaterial(0, BarMaterial);
		}
	}

	const double InformationGain = Model.ComputeInformationGain(CurrentProbabilities, AncestralProbabilities);
	LogInformationGain(InformationGain);

	VisualizeProbabilities();
}

void AInformationGainVisualizerActor::VisualizeProbabilities()
{
	if (!BarMesh || CurrentProbabilities.Num() == 0)
	{
		return;
	}

	BarInstances->ClearInstances();

	for (int32 Index = 0; Index < CurrentProbabilities.Num(); ++Index)
	{
		const float Probability = static_cast<float>(CurrentProbabilities[Index]);
		const float Height = FMath::Max(Probability * VisualizationScale, 5.0f);
		const float LocationX = Index * BarSpacing;
		const FVector Location(LocationX, 0.0f, Height * 0.5f);
		const FVector Scale(BarWidth * 0.01f, BarWidth * 0.01f, Height * 0.01f);
		const FTransform InstanceTransform(FRotator::ZeroRotator, Location, Scale);
		BarInstances->AddInstance(InstanceTransform);
	}
}

void AInformationGainVisualizerActor::LogInformationGain(double Gain) const
{
	UE_LOG(LogTemp, Log, TEXT("Phylogenetic Information Gain: %.4f"), Gain);
}

void AInformationGainVisualizerActor::EnsureProbabilitySizesMatch() const
{
	if (CurrentProbabilities.Num() != AncestralProbabilities.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Current and ancestral probability arrays must have the same length."));
	}
}
