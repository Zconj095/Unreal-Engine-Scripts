#include "NeuralPlasticitySeasonalAdaptationComponent.h"
#include "Logging/LogMacros.h"

UNeuralPlasticitySeasonalAdaptationComponent::UNeuralPlasticitySeasonalAdaptationComponent()
{
	PlasticityCoefficients.Init(1.0f, 4);
	Amplitudes.Init(1.0f, 4);
	Frequencies.Init(1.0f, 4);
	PhaseShifts.Init(0.0f, 4);
	IntegrationSteps = 1000;
}

void UNeuralPlasticitySeasonalAdaptationComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 Season = 0; Season < 4; ++Season)
	{
		const float DeltaNeuralPlasticity = IntegrateSeasonalPlasticity(Season);
		UE_LOG(LogTemp, Log, TEXT("?N_season for season %d: %0.4f"), Season + 1, DeltaNeuralPlasticity);
	}
}

float UNeuralPlasticitySeasonalAdaptationComponent::CalculateSeasonalResonance(int32 Season, float Time) const
{
	const float Alpha = Amplitudes.IsValidIndex(Season) ? Amplitudes[Season] : 1.0f;
	const float Freq = Frequencies.IsValidIndex(Season) ? Frequencies[Season] : 1.0f;
	const float Phase = PhaseShifts.IsValidIndex(Season) ? PhaseShifts[Season] : 0.0f;
	return Alpha * FMath::Cos(2.0f * PI * Freq * Time + Phase);
}

float UNeuralPlasticitySeasonalAdaptationComponent::IntegrateSeasonalPlasticity(int32 Season) const
{
	if (!PlasticityCoefficients.IsValidIndex(Season) || IntegrationSteps <= 0)
	{
		return 0.0f;
	}

	const float Kappa = PlasticityCoefficients[Season];
	const float DurationSeconds = SeasonalDuration * TimeScale;
	const float StepSize = DurationSeconds / IntegrationSteps;
	float Accumulator = 0.0f;

	auto Evaluate = [&](float Time) -> float
	{
		return Kappa * CalculateSeasonalResonance(Season, Time);
	};

	Accumulator += 0.5f * (Evaluate(0.0f) + Evaluate(DurationSeconds));
	for (int32 Step = 1; Step < IntegrationSteps; ++Step)
	{
		const float SampleTime = Step * StepSize;
		Accumulator += Evaluate(SampleTime);
	}

	return Accumulator * StepSize;
}
