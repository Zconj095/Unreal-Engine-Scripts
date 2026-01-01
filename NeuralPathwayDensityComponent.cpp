#include "NeuralPathwayDensityComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralPathwayDensityComponent::UNeuralPathwayDensityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralPathwayDensityComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdatePathwayDensity();
}

void UNeuralPathwayDensityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePathwayDensity();
}

void UNeuralPathwayDensityComponent::UpdatePathwayDensity()
{
	if (CrystallineComplexity <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Crystalline complexity must be greater than zero; defaulting to 1."));
		CrystallineComplexity = 1.0f;
	}

	NeuralPathwayDensity = Alpha * FMath::Loge(CrystallineComplexity) + Beta;
	UE_LOG(LogTemp, Log, TEXT("Neural Pathway Density (N_p): %.4f"), NeuralPathwayDensity);
}
