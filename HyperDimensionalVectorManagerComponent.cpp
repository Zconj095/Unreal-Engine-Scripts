#include "HyperDimensionalVectorManagerComponent.h"

#include "Logging/LogMacros.h"

UHyperDimensionalVectorManagerComponent::UHyperDimensionalVectorManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHyperDimensionalVectorManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	auto EnsureVector = [&](UHyperDimensionalVectorComponent*& Vec) {
		if (!Vec)
		{
			Vec = NewObject<UHyperDimensionalVectorComponent>(Owner);
			if (Vec)
			{
				Vec->RegisterComponent();
			}
		}
	};

	EnsureVector(HV1);
	EnsureVector(HV2);

	if (!HV1 || !HV2)
	{
		UE_LOG(LogTemp, Error, TEXT("HyperDimensionalVectorManagerComponent: Failed to create vector components."));
		return;
	}

	HV1->Initialize(VectorDimensions);
	HV2->Initialize(VectorDimensions);

	UE_LOG(LogTemp, Log, TEXT("Initializing HyperDimensional Vectors..."));

	HV1->SetDimension(0, 1.0f);
	HV1->SetDimension(4, 0.5f);
	HV1->Normalize();

	HV2->SetDimension(0, 0.7f);
	HV2->SetDimension(4, 0.8f);

	const float Dot = HV1->DotProduct(HV2);
	UE_LOG(LogTemp, Log, TEXT("Dot Product of HV1 and HV2: %.4f"), Dot);
}
