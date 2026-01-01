#include "PsychologicalResonanceComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPsychologicalResonanceComponent::UPsychologicalResonanceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPsychologicalResonanceComponent::BeginPlay()
{
	Super::BeginPlay();
	Resonance = CalculatePsychologicalResonance(0.0f);
}

void UPsychologicalResonanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime += DeltaTime;
	Resonance = CalculatePsychologicalResonance(CurrentTime);
	UE_LOG(LogTemp, Log, TEXT("Psychological Resonance (R_p) at time %0.2f: %0.4f"), CurrentTime, Resonance);
}

float UPsychologicalResonanceComponent::CalculatePsychologicalResonance(float Time) const
{
	return Amplitude * FMath::Sin(2.0f * PI * Frequency * Time + Phase);
}
