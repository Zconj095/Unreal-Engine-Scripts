#include "NeuroadaptiveLearningEfficiencyComponent.h"
#include "Logging/LogMacros.h"

UNeuroadaptiveLearningEfficiencyComponent::UNeuroadaptiveLearningEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuroadaptiveLearningEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	RecalculateEfficiency();
}

void UNeuroadaptiveLearningEfficiencyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RecalculateEfficiency();
	UE_LOG(LogTemp, Log, TEXT("NLE: %0.4f"), NeuroadaptiveLearningEfficiency);
}

void UNeuroadaptiveLearningEfficiencyComponent::UpdateEta(float NewEta)
{
	Eta = NewEta;
	RecalculateEfficiency();
}

void UNeuroadaptiveLearningEfficiencyComponent::UpdateActiveNeurons(float NewActiveNeurons)
{
	ActiveNeurons = NewActiveNeurons;
	RecalculateEfficiency();
}

void UNeuroadaptiveLearningEfficiencyComponent::UpdateSynapticDensity(float NewSynapticDensity)
{
	SynapticDensity = NewSynapticDensity;
	RecalculateEfficiency();
}

void UNeuroadaptiveLearningEfficiencyComponent::UpdateTimeTaken(float NewTime)
{
	if (NewTime > 0.0f)
	{
		TimeTaken = NewTime;
		RecalculateEfficiency();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Time taken must be greater than zero."));
	}
}

void UNeuroadaptiveLearningEfficiencyComponent::UpdateMetabolicCost(float NewCost)
{
	if (NewCost > 0.0f)
	{
		MetabolicCost = NewCost;
		RecalculateEfficiency();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Metabolic cost must be greater than zero."));
	}
}

float UNeuroadaptiveLearningEfficiencyComponent::GetNeuroadaptiveLearningEfficiency() const
{
	return NeuroadaptiveLearningEfficiency;
}

void UNeuroadaptiveLearningEfficiencyComponent::RecalculateEfficiency()
{
	if (TimeTaken <= 0.0f || MetabolicCost <= 0.0f)
	{
		NeuroadaptiveLearningEfficiency = 0.0f;
		return;
	}

	NeuroadaptiveLearningEfficiency = (Eta * ActiveNeurons * SynapticDensity) / (TimeTaken * MetabolicCost);
}
