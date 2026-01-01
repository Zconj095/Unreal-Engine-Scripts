#include "EnergyFlowVisualizer1Component.h"

#include "Components/MeshComponent.h"
#include "Kismet/GameplayStatics.h"

UEnergyFlowVisualizer1Component::UEnergyFlowVisualizer1Component()
{
	PrimaryComponentTick.bCanEverTick = false;
	MagicalContribution = 100.0f;
	HeatStart = 400.0f;
	HeatEnd = 700.0f;
	HeatStep = 100.0f;
	WorkStart = 150.0f;
	WorkEnd = 300.0f;
	WorkStep = 50.0f;
}

void UEnergyFlowVisualizer1Component::BeginPlay()
{
	Super::BeginPlay();
	VisualizeGrid();
}

void UEnergyFlowVisualizer1Component::VisualizeGrid()
{
	if (!IndicatorActorClass || !GetWorld())
	{
		return;
	}

	for (float HeatTransfer = HeatStart; HeatTransfer <= HeatEnd; HeatTransfer += HeatStep)
	{
		for (float WorkDone = WorkStart; WorkDone <= WorkEnd; WorkDone += WorkStep)
		{
			const double EnergyChange = CalculateEnergyChange(HeatTransfer, WorkDone, MagicalContribution);
			const FVector Location(HeatTransfer / 100.0f, EnergyChange / 50.0f, WorkDone / 50.0f);
			AActor* Indicator = GetWorld()->SpawnActor<AActor>(IndicatorActorClass, Location, FRotator::ZeroRotator);
			if (!Indicator)
			{
				continue;
			}

			Indicator->SetActorScale3D(FVector(EnergyChange / 100.0));

			TInlineComponentArray<UMeshComponent*> MeshComponents;
			Indicator->GetComponents(MeshComponents);
			for (UMeshComponent* MeshComponent : MeshComponents)
			{
				const FLinearColor Color = FLinearColor::LerpUsingHSV(FLinearColor::Blue, FLinearColor::Red, EnergyChange / 500.0);
				MeshComponent->SetVectorParameterValueOnMaterials(FName("Color"), FVector(Color.R, Color.G, Color.B));
			}
		}
	}
}

double UEnergyFlowVisualizer1Component::CalculateEnergyChange(double HeatTransfer, double WorkDone, double InMagicalContribution) const
{
	return HeatTransfer + WorkDone + InMagicalContribution;
}
