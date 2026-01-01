#include "RunicCoreComponent.h"
#include "Logging/LogMacros.h"
#include <cmath>

URunicCoreComponent::URunicCoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunicCoreComponent::BeginPlay()
{
	Super::BeginPlay();

	const double Result = ComputeRunicValue(Steps);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Log, TEXT("Runic Core R(%d) = %.6f"), Steps, Result);
	}
}

double URunicCoreComponent::ComputeRunicValue(int32 Step) const
{
	if (Step <= 0)
	{
		return 1.0;
	}

	const double Prev = ComputeRunicValue(Step - 1);
	return std::tgamma(Prev + 1.0);
}
