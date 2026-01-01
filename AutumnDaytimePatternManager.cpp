#include "AutumnDaytimePatternManager.h"

AAutumnDaytimePatternManager::AAutumnDaytimePatternManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAutumnDaytimePatternManager::BeginPlay()
{
    Super::BeginPlay();

    // Initialize patterns with example data
    InitializePatterns();

    // Check active patterns
    CheckActivePatterns(CreaturePatterns, TEXT("Creature Patterns"));
    CheckActivePatterns(TreePatterns, TEXT("Tree Patterns"));
}

void AAutumnDaytimePatternManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Simulate time progression
    SimulateTime(DeltaSeconds);

    // Dynamically check active patterns
    CheckActivePatterns(FlowerPatterns, TEXT("Flower Patterns"));
    CheckActivePatterns(WeatherPatterns, TEXT("Weather Patterns"));
}

void AAutumnDaytimePatternManager::InitializePatterns()
{
    CreaturePatterns.Add(FAutumnDaytimePattern(
        TEXT("Squirrels Gathering Nuts"),
        8.f, 16.f,
        TEXT("Squirrels actively gather nuts during autumn days.")
    ));

    TreePatterns.Add(FAutumnDaytimePattern(
        TEXT("Maple Leaves Falling"),
        9.f, 17.f,
        TEXT("Maple trees shed their colorful leaves throughout the day in autumn.")
    ));

    BerryPatterns.Add(FAutumnDaytimePattern(
        TEXT("Cranberry Growth"),
        10.f, 14.f,
        TEXT("Cranberries grow rapidly during autumn daylight hours.")
    ));

    FlowerPatterns.Add(FAutumnDaytimePattern(
        TEXT("Autumn Marigold Bloom"),
        7.f, 15.f,
        TEXT("Marigolds bloom under the gentle autumn sun.")
    ));

    ItemPatterns.Add(FAutumnDaytimePattern(
        TEXT("Seasonal Decorations"),
        10.f, 16.f,
        TEXT("Special autumn decorations appear during the daytime.")
    ));

    MoonPhasePatterns.Add(FAutumnDaytimePattern(
        TEXT("Harvest Moon Effects"),
        12.f, 13.f,
        TEXT("The harvest moon subtly influences the environment during autumn days.")
    ));

    WeatherPatterns.Add(FAutumnDaytimePattern(
        TEXT("Cool Breezes"),
        11.f, 18.f,
        TEXT("Gentle cool breezes sweep through the land during autumn days.")
    ));

    RoutePatterns.Add(FAutumnDaytimePattern(
        TEXT("Forest Path Visibility"),
        8.f, 18.f,
        TEXT("Forest paths are most visible under autumn daylight.")
    ));

    HabitatPatterns.Add(FAutumnDaytimePattern(
        TEXT("Bird Migration"),
        9.f, 17.f,
        TEXT("Birds begin their migration patterns during autumn daylight hours.")
    ));

    BiomePatterns.Add(FAutumnDaytimePattern(
        TEXT("Forest Color Changes"),
        10.f, 16.f,
        TEXT("Forests undergo vivid color changes during autumn days.")
    ));

    EcosystemPatterns.Add(FAutumnDaytimePattern(
        TEXT("Wetland Activity"),
        7.f, 13.f,
        TEXT("Wetlands are most vibrant during the early autumn mornings.")
    ));

    RegionPatterns.Add(FAutumnDaytimePattern(
        TEXT("Mountain Mist"),
        9.f, 12.f,
        TEXT("Mist blankets mountain regions during the morning hours of autumn.")
    ));
}

void AAutumnDaytimePatternManager::SimulateTime(float DeltaSeconds)
{
    // Increment current hour (1 second = 1 in-game minute)
    CurrentHour += DeltaSeconds / 60.f;

    // Wrap around the hour if it exceeds 24
    if (CurrentHour >= 24.f)
    {
        CurrentHour -= 24.f;
    }
}

void AAutumnDaytimePatternManager::CheckActivePatterns(const TArray<FAutumnDaytimePattern>& Patterns, const FString& Category) const
{
    UE_LOG(LogTemp, Log, TEXT("--- Checking %s ---"), *Category);
    for (const FAutumnDaytimePattern& Pattern : Patterns)
    {
        if (Pattern.IsActive(CurrentHour))
        {
            UE_LOG(LogTemp, Log, TEXT("Active Pattern: %s - %s"), *Pattern.EntityName, *Pattern.Description);
        }
    }
}

