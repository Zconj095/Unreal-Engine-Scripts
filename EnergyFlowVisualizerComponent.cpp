#include "EnergyFlowVisualizerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"

UEnergyFlowVisualizerComponent::UEnergyFlowVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MagicalField = FVector(1.0f, 0.0f, 0.0f);
	Areas = {1.0f, 2.0f, 1.5f};
	Normals = {FVector(1.0f,0.0f,0.0f), FVector(0.0f,1.0f,0.0f), FVector(0.0f,0.0f,1.0f)};
}

void UEnergyFlowVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	const double TotalFlux = ComputeTotalFlux(Areas, Normals);
	UE_LOG(LogTemp, Log, TEXT("Total Energy Flux: %.4f"), TotalFlux);

	VisualizeSurfaces();
}

double UEnergyFlowVisualizerComponent::ComputeTotalFlux(const TArray<float>& InAreas, const TArray<FVector>& InNormals) const
{
	if (InAreas.Num() != InNormals.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Area and normal arrays must share the same length."));
		return 0.0;
	}

	double TotalFlux = 0.0;
	for (int32 Index = 0; Index < InAreas.Num(); ++Index)
	{
		const float Area = InAreas[Index];
		const FVector Normal = InNormals[Index].GetSafeNormal();
		TotalFlux += Area * FVector::DotProduct(Normal, MagicalField);
	}
	return TotalFlux;
}

void UEnergyFlowVisualizerComponent::VisualizeSurfaces()
{
	if (!SurfaceMesh || !GetWorld())
	{
		return;
	}

	for (int32 Index = 0; Index < Areas.Num(); ++Index)
	{
		const FVector Normal = Normals.IsValidIndex(Index) ? Normals[Index].GetSafeNormal() : FVector::UpVector;
		const FVector Position = Normal * Areas[Index];

		AStaticMeshActor* SurfaceActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Position, FRotator::ZeroRotator);
		if (!SurfaceActor)
		{
			continue;
		}

		UStaticMeshComponent* MeshComponent = SurfaceActor->GetStaticMeshComponent();
		if (!MeshComponent)
		{
			SurfaceActor->Destroy();
			continue;
		}

		MeshComponent->SetStaticMesh(SurfaceMesh);
		if (SurfaceMaterial)
		{
			MeshComponent->SetMaterial(0, SurfaceMaterial);
		}

		const float Scale = Areas[Index];
		MeshComponent->SetWorldScale3D(FVector(Scale));
	}
}
