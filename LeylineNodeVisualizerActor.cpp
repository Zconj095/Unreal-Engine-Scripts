#include "LeylineNodeVisualizerActor.h"
#include "Engine/Engine.h"
#include "Engine/CollisionProfile.h"

ALeylineNodeVisualizerActor::ALeylineNodeVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ProcessingComponent = CreateDefaultSubobject< UDistributedMagicalProcessing >(TEXT("ProcessingComponent"));

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	NodeInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PointInstances"));
	NodeInstances->SetupAttachment(RootComponent);
	NodeInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ALeylineNodeVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (!ProcessingComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProcessingComponent missing."));
		return;
	}

	ProcessingComponent->InitializeNodes(NodeCount);
	const float TotalPower = ProcessingComponent->ComputeTotalPower();
	UE_LOG(LogTemp, Log, TEXT("Total Computational Power: %.4f"), TotalPower);

	if (!NodeMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("NodeMesh is not configured; skipping mesh visualization."));
		return;
	}

	NodeInstances->SetStaticMesh(NodeMesh);
	if (NodeMaterial)
	{
		NodeInstances->SetMaterial(0, NodeMaterial);
	}

	VisualizeNodes();
}

void ALeylineNodeVisualizerActor::VisualizeNodes()
{
	NodeInstances->ClearInstances();

	for (int32 Index = 0; Index < NodeCount; ++Index)
	{
		const float Capability = ProcessingComponent->GetProcessingCapability(Index);
		const FVector Gradient = ProcessingComponent->GetEnergyGradient(Index);
		const FVector Location = Gradient * NodeSpacing;
		const FVector Scale(FMath::Max(Capability * 0.1f, 0.1f));
		FTransform Transform(FRotator::ZeroRotator, Location, Scale);
		NodeInstances->AddInstance(Transform);
	}
}
