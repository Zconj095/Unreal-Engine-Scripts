#include "NeuralTerrainComplexityIndexComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeuralTerrainComplexityIndexComponent::UNeuralTerrainComplexityIndexComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralTerrainComplexityIndexComponent::BeginPlay()
{
	Super::BeginPlay();

	NTCIValue = CalculateNTCI();
	UE_LOG(LogTemp, Log, TEXT("NTCI: %0.4f"), NTCIValue);
}

void UNeuralTerrainComplexityIndexComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	NTCIValue = CalculateNTCI();
	UE_LOG(LogTemp, Log, TEXT("NTCI: %0.4f"), NTCIValue);
}

void UNeuralTerrainComplexityIndexComponent::UpdateTerrainWeights(const TArray<float>& NewWeights)
{
	TerrainWeights = NewWeights;
}

void UNeuralTerrainComplexityIndexComponent::UpdateDimensionalComplexities(const TArray<float>& NewComplexities)
{
	DimensionalComplexities = NewComplexities;
}

float UNeuralTerrainComplexityIndexComponent::CalculateNTCI() const
{
	if (TerrainWeights.Num() != DimensionalComplexities.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Terrain weights and complexities must have the same length."));
		return 0.0f;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < TerrainWeights.Num(); ++Index)
	{
		const float Complexity = DimensionalComplexities[Index];
		if (Complexity <= 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Dimensional complexity at index %d must be greater than zero."), Index);
			continue;
		}

		Sum += TerrainWeights[Index] * FMath::Log2(Complexity);
	}

	return Beta * Sum;
}
