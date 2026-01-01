#include "InteractiveDesignDivergenceRatioComponent.h"
#include "Engine/Engine.h"

UInteractiveDesignDivergenceRatioComponent::UInteractiveDesignDivergenceRatioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractiveDesignDivergenceRatioComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateIDDR();
}

void UInteractiveDesignDivergenceRatioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RecalculateIDDR();
}

bool UInteractiveDesignDivergenceRatioComponent::bHasValidOmega() const
{
	return Omega > KINDA_SMALL_NUMBER;
}

float UInteractiveDesignDivergenceRatioComponent::CalculateIDDRValue() const
{
	if (!bHasValidOmega())
	{
		return 0.0f;
	}

	return (DeltaPsi / Omega) * Lambda;
}

void UInteractiveDesignDivergenceRatioComponent::RecalculateIDDR()
{
	if (!bHasValidOmega())
	{
		UE_LOG(LogTemp, Error, TEXT("Total cognitive energy (Ω) must be greater than zero."));
		IDDR = 0.0f;
		return;
	}

	IDDR = CalculateIDDRValue();
	UE_LOG(LogTemp, Log, TEXT("IDDR: %.4f"), IDDR);
}

void UInteractiveDesignDivergenceRatioComponent::UpdateDeltaPsi(float NewDeltaPsi)
{
	DeltaPsi = NewDeltaPsi;
	RecalculateIDDR();
}

void UInteractiveDesignDivergenceRatioComponent::UpdateOmega(float NewOmega)
{
	if (NewOmega <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Total cognitive energy (Ω) must be greater than zero."));
		return;
	}

	Omega = NewOmega;
	RecalculateIDDR();
}

void UInteractiveDesignDivergenceRatioComponent::UpdateLambda(float NewLambda)
{
	Lambda = NewLambda;
	RecalculateIDDR();
}

float UInteractiveDesignDivergenceRatioComponent::GetIDDR() const
{
	return IDDR;
}
