#include "ReactionModelingComplexityComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UReactionModelingComplexityComponent::UReactionModelingComplexityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UReactionModelingComplexityComponent::BeginPlay()
{
	Super::BeginPlay();
	RMCResult = CalculateRMC();
	UE_LOG(LogTemp, Log, TEXT("Reaction Modeling Complexity (RMC): %0.2f"), RMCResult);
}

void UReactionModelingComplexityComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float NewResult = CalculateRMC();
	if (!FMath::IsNearlyEqual(RMCResult, NewResult))
	{
		RMCResult = NewResult;
		UE_LOG(LogTemp, Log, TEXT("Reaction Modeling Complexity (RMC) updated: %0.2f"), RMCResult);
	}
}

float UReactionModelingComplexityComponent::CalculateRMC() const
{
	if (ReactionRates.Num() != Complexities.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ReactionRates and Complexities must be the same length."));
		return 0.0f;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < ReactionRates.Num(); ++Index)
	{
		const float Ci = Complexities[Index];
		if (Ci > 0.0f)
		{
			Sum += ReactionRates[Index] * FMath::Log2(Ci);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Complexity[%d] must be greater than zero."), Index);
		}
	}

	return Beta * Sum;
}
