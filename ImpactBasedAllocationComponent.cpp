#include "ImpactBasedAllocationComponent.h"
#include "Math/UnrealMathUtility.h"

UImpactBasedAllocationComponent::UImpactBasedAllocationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UImpactBasedAllocationComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeAreas();
	NormalizeImpactScores();
	AllocateResources();
	FineTuneAllocations();
	LogAllocations();
}

void UImpactBasedAllocationComponent::InitializeAreas()
{
	if (Areas.Num() == 0)
	{
		Areas.Add({TEXT("Area 1"), 10.0f});
		Areas.Add({TEXT("Area 2"), 20.0f});
		Areas.Add({TEXT("Area 3"), 15.0f});
	}
}

void UImpactBasedAllocationComponent::NormalizeImpactScores()
{
	float TotalImpact = 0.0f;
	for (const FImpactArea& Area : Areas)
	{
		TotalImpact += Area.ImpactScore;
	}
	if (TotalImpact <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	for (FImpactArea& Area : Areas)
	{
		Area.NormalizedScore = Area.ImpactScore / TotalImpact;
	}
}

void UImpactBasedAllocationComponent::AllocateResources()
{
	for (FImpactArea& Area : Areas)
	{
		Area.AllocatedResources = Area.NormalizedScore * TotalResources;
	}
}

void UImpactBasedAllocationComponent::FineTuneAllocations()
{
	for (FImpactArea& Area : Areas)
	{
		const float Adjustment = FMath::FRandRange(-FineTuneFactor, FineTuneFactor);
		Area.AllocatedResources += Area.AllocatedResources * Adjustment;
		Area.AllocatedResources = FMath::Clamp(Area.AllocatedResources, 0.0f, TotalResources);
	}
}

void UImpactBasedAllocationComponent::LogAllocations() const
{
	UE_LOG(LogTemp, Log, TEXT("Final Allocations:"));
	for (const FImpactArea& Area : Areas)
	{
		UE_LOG(LogTemp, Log, TEXT("Area: %s | Resources: %.2f | Normalized Score: %.2f"),
		       *Area.Name, Area.AllocatedResources, Area.NormalizedScore);
	}
}
