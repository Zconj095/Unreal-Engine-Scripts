#include "CelestialCalculations.h"

ACelestialCalculations::ACelestialCalculations()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACelestialCalculations::BeginPlay()
{
    Super::BeginPlay();

    // Instantiate Astronomical and Astrological calculations
    AstronomicalCalculation = FAstronomical(
        TEXT("Universe Expansion Study"),
        TEXT("Calculates values based on cosmic scale parameters, like galaxy distances and spacetime curvature.")
    );

    AstrologicalCalculation = FAstrological(
        TEXT("Zodiac Influence Analysis"),
        TEXT("Quantum-based calculation for planetary alignments affecting astrological predictions.")
    );

    // Instantiate Clocks
    AstronomicalClock = FCelestialClock(
        TEXT("Astronomical Clock"),
        TEXT("Tracks universal timing and massive-scale cosmic calculations."),
        12.f, // Start at noon
        1.f   // Increment by 1 hour per update
    );

    AstrologicalClock = FCelestialClock(
        TEXT("Astrological Clock"),
        TEXT("Tracks astrological timing and quantum-based events."),
        0.f, // Start at midnight
        2.f  // Increment by 2 hours per update
    );

    // Perform initial calculations
    AstronomicalCalculation.PerformCalculation();
    AstrologicalCalculation.PerformCalculation();

    // Display initial clock times
    AstronomicalClock.UpdateTime();
    AstrologicalClock.UpdateTime();
}

void ACelestialCalculations::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Simulate clock updates in real-time: every second
    SecondsAccumulator += DeltaSeconds;
    if (SecondsAccumulator >= UpdateIntervalSeconds)
    {
        SecondsAccumulator -= UpdateIntervalSeconds;

        AstronomicalClock.UpdateTime();
        UE_LOG(LogTemp, Log, TEXT("Astronomical Clock TimeFrame: %s"), *AstronomicalClock.GetTimeFrame());

        AstrologicalClock.UpdateTime();
        UE_LOG(LogTemp, Log, TEXT("Astrological Clock TimeFrame: %s"), *AstrologicalClock.GetTimeFrame());
    }
}

