#include "AutumnNighttimePatternManager.h"

AAutumnNighttimePatternManager::AAutumnNighttimePatternManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAutumnNighttimePatternManager::BeginPlay()
{
    Super::BeginPlay();

    // Initialize patterns with example data
    InitializePatterns();

    // Check active patterns
    CheckActivePatterns(CreaturePatterns, TEXT("Creature Patterns"));
    CheckActivePatterns(TreePatterns, TEXT("Tree Patterns"));
}

void AAutumnNighttimePatternManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Simulate time progression
    SimulateTime(DeltaSeconds);

    // Dynamically check active patterns
    CheckActivePatterns(FlowerPatterns, TEXT("Flower Patterns"));
    CheckActivePatterns(WeatherPatterns, TEXT("Weather Patterns"));
}

void AAutumnNighttimePatternManager::InitializePatterns()
{
    CreaturePatterns.Add(FAutumnNighttimePattern(
        TEXT("Owls Hunting"),
        20.f, 4.f,
        TEXT("Owls actively hunt small prey during autumn nights.")
    ));

    TreePatterns.Add(FAutumnNighttimePattern(
        TEXT("Moonlit Birch Leaves"),
        22.f, 2.f,
        TEXT("Birch trees glow faintly under the moonlight in autumn.")
    ));

    BerryPatterns.Add(FAutumnNighttimePattern(
        TEXT("Cranberry Ripening"),
        23.f, 5.f,
        TEXT("Cranberries ripen steadily during cool autumn nights.")
    ));

    FlowerPatterns.Add(FAutumnNighttimePattern(
        TEXT("Autumn Jasmine Bloom"),
        21.f, 3.f,
        TEXT("Jasmine flowers emit their fragrance during autumn nights.")
    ));

    ItemPatterns.Add(FAutumnNighttimePattern(
        TEXT("Hidden Autumn Treasures"),
        19.f, 1.f,
        TEXT("Special treasures appear during the night in autumn forests.")
    ));

    MoonPhasePatterns.Add(FAutumnNighttimePattern(
        TEXT("Harvest Moonlight Effects"),
        20.f, 23.f,
        TEXT("The harvest moon casts a golden glow during autumn nights.")
    ));

    WeatherPatterns.Add(FAutumnNighttimePattern(
        TEXT("Cool Autumn Winds"),
        0.f, 6.f,
        TEXT("Gentle cool winds sweep across the fields during autumn nights.")
    ));

    RoutePatterns.Add(FAutumnNighttimePattern(
        TEXT("Dimly Lit Forest Paths"),
        21.f, 4.f,
        TEXT("Forest paths are dimly lit during autumn nights.")
    ));

    HabitatPatterns.Add(FAutumnNighttimePattern(
        TEXT("Nocturnal Forest Creatures"),
        20.f, 5.f,
        TEXT("Nocturnal animals thrive in the forest during autumn nights.")
    ));

    BiomePatterns.Add(FAutumnNighttimePattern(
        TEXT("Wetland Evening Activity"),
        19.f, 3.f,
        TEXT("Wetlands show vibrant activity during autumn nights.")
    ));

    EcosystemPatterns.Add(FAutumnNighttimePattern(
        TEXT("Nocturnal Pollination"),
        20.f, 4.f,
        TEXT("Pollinators such as moths are active during autumn nights.")
    ));

    RegionPatterns.Add(FAutumnNighttimePattern(
        TEXT("Mountain Star Visibility"),
        21.f, 2.f,
        TEXT("Stars are most visible in mountain regions during autumn nights.")
    ));
}

void AAutumnNighttimePatternManager::SimulateTime(float DeltaSeconds)
{
    // Increment current hour (1 second = 1 in-game minute)
    CurrentHour += DeltaSeconds / 60.f;

    // Wrap around the hour if it exceeds 24
    if (CurrentHour >= 24.f)
    {
        CurrentHour -= 24.f;
    }
}

void AAutumnNighttimePatternManager::CheckActivePatterns(const TArray<FAutumnNighttimePattern>& Patterns, const FString& Category) const
{
    UE_LOG(LogTemp, Log, TEXT("--- Checking %s ---"), *Category);
    for (const FAutumnNighttimePattern& Pattern : Patterns)
    {
        if (Pattern.IsActive(CurrentHour))
        {
            UE_LOG(LogTemp, Log, TEXT("Active Pattern: %s - %s"), *Pattern.EntityName, *Pattern.Description);
        }
    }
}

