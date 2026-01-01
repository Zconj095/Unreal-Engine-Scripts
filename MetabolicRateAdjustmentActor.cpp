#include "MetabolicRateAdjustmentActor.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

AMetabolicRateAdjustmentActor::AMetabolicRateAdjustmentActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMetabolicRateAdjustmentActor::BeginPlay()
{
	Super::BeginPlay();

	const float Rate = CalculateMetabolicRate(Duration);
	UE_LOG(LogTemp, Log, TEXT("Adjusted Metabolic Rate (M): %.4f"), Rate);
}

void AMetabolicRateAdjustmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Elapsed += DeltaTime;
	DrawDebugSphere(GetWorld(), FVector(Elapsed * 10.f, CalculateMetabolicRate(Elapsed), 0.f), 20.f, 8, FColor::Green, false, -1.f);
}

float AMetabolicRateAdjustmentActor::CalculateMetabolicRate(float TimeSeconds) const
{
	return InitialRate * FMath::Exp(-DecayConstant * TimeSeconds) + MagicalInfusion;
}
