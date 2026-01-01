#include "EnviroPatterns.h"

AEnviroPatterns::AEnviroPatterns()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnviroPatterns::BeginPlay()
{
    Super::BeginPlay();

    // Define some lunar and solar patterns (sample data)
    CelestialPatterns.Add(FCelestialPattern(
        EPatternType::Lunar,
        TEXT("Full Moon Night"),
        TEXT("Bright moonlight affects nocturnal activities and visibility."),
        21.f, // 9 PM
        5.f,  // 5 AM
        TEXT("Increased visibility, cooler temperature")
    ));

    CelestialPatterns.Add(FCelestialPattern(
        EPatternType::Solar,
        TEXT("Solar Noon"),
        TEXT("Peak sunlight influencing temperature and shadow length."),
        12.f, // Noon
        13.f, // 1 PM
        TEXT("Maximum temperature, high light intensity")
    ));

    CelestialPatterns.Add(FCelestialPattern(
        EPatternType::Lunar,
        TEXT("New Moon Night"),
        TEXT("Minimal moonlight impacts nocturnal visibility and activity."),
        20.f, // 8 PM
        6.f,  // 6 AM
        TEXT("Reduced visibility, colder temperatures")
    ));

    // Display pattern details
    for (const FCelestialPattern& Pattern : CelestialPatterns)
    {
        Pattern.DisplayDetails();
    }
}

void AEnviroPatterns::SimulatePatterns(float CurrentHour) const
{
    UE_LOG(LogTemp, Log, TEXT("Simulating patterns for time: %.0f:00"), CurrentHour);

    for (const FCelestialPattern& Pattern : CelestialPatterns)
    {
        if (Pattern.IsActive(CurrentHour))
        {
            UE_LOG(LogTemp, Log, TEXT("Active Pattern: %s"), *Pattern.Name);
            UE_LOG(LogTemp, Log, TEXT("Effects: %s"), *Pattern.Effects);
        }
    }
}

