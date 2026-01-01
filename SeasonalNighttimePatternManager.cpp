#include "SeasonalNighttimePatternManager.h"

ASeasonalNighttimePatternManager::ASeasonalNighttimePatternManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASeasonalNighttimePatternManager::BeginPlay()
{
    Super::BeginPlay();

    // Initialize patterns with example data
    InitializePatterns();

    // Check active patterns for each category
    CheckActivePatterns(CreaturePatterns, TEXT("Creature Patterns"));
    CheckActivePatterns(TreePatterns, TEXT("Tree Patterns"));
}

void ASeasonalNighttimePatternManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Simulate time progression
    SimulateTime(DeltaSeconds);

    // Dynamically check active patterns
    CheckActivePatterns(FlowerPatterns, TEXT("Flower Patterns"));
    CheckActivePatterns(WeatherPatterns, TEXT("Weather Patterns"));
}

void ASeasonalNighttimePatternManager::InitializePatterns()
{
    CreaturePatterns.Add(FSeasonalNighttimePattern(
        TEXT("Bat Foraging"),
        ESeason::Spring,
        20.f, 4.f,
        TEXT("Bats forage for food during spring nights.")
    ));

    TreePatterns.Add(FSeasonalNighttimePattern(
        TEXT("Cherry Blossom Moonlight Bloom"),
        ESeason::Spring,
        22.f, 2.f,
        TEXT("Cherry blossoms bloom beautifully under moonlight during spring nights.")
    ));

    BerryPatterns.Add(FSeasonalNighttimePattern(
        TEXT("Blueberry Nocturnal Growth"),
        ESeason::Summer,
        23.f, 5.f,
        TEXT("Blueberries grow rapidly during summer nights.")
    ));

    FlowerPatterns.Add(FSeasonalNighttimePattern(
        TEXT("Night-blooming Jasmine"),
        ESeason::Autumn,
        19.f, 3.f,
        TEXT("Jasmine flowers release their fragrance at night during autumn.")
    ));

    MoonPhasePatterns.Add(FSeasonalNighttimePattern(
        TEXT("Full Moonlight Effects"),
        ESeason::Winter,
        21.f, 1.f,
        TEXT("Full moonlight enhances the environment during winter nights.")
    ));

    WeatherPatterns.Add(FSeasonalNighttimePattern(
        TEXT("Winter Night Snowfall"),
        ESeason::Winter,
        0.f, 6.f,
        TEXT("Snowfall is frequent during winter nights.")
    ));
}

void ASeasonalNighttimePatternManager::SimulateTime(float DeltaSeconds)
{
    // Increment current hour (1 second = 1 in-game minute)
    CurrentHour += DeltaSeconds / 60.f;

    // Wrap around the hour if it exceeds 24
    if (CurrentHour >= 24.f)
    {
        CurrentHour -= 24.f;
    }
}

void ASeasonalNighttimePatternManager::CheckActivePatterns(const TArray<FSeasonalNighttimePattern>& Patterns, const FString& Category) const
{
    UE_LOG(LogTemp, Log, TEXT("--- Checking %s ---"), *Category);
    for (const FSeasonalNighttimePattern& Pattern : Patterns)
    {
        if (Pattern.IsActive(CurrentHour, CurrentSeason))
        {
            UE_LOG(LogTemp, Log, TEXT("Active Pattern: %s - %s"), *Pattern.EntityName, *Pattern.Description);
        }
    }
}

