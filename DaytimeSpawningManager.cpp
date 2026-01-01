#include "DaytimeSpawningManager.h"

ADaytimeSpawningManager::ADaytimeSpawningManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ADaytimeSpawningManager::BeginPlay()
{
    Super::BeginPlay();

    // Initialize example patterns
    InitializePatterns();

    // Check for active spawn patterns
    CheckActivePatterns(TreePatterns, TEXT("Tree Patterns"));
    CheckActivePatterns(BerryPatterns, TEXT("Berry Patterns"));
}

void ADaytimeSpawningManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Simulate time progression
    SimulateTime(DeltaSeconds);

    // Check active patterns
    CheckActivePatterns(FlowerPatterns, TEXT("Flower Patterns"));
    CheckActivePatterns(CreaturePatterns, TEXT("Creature Patterns"));
}

void ADaytimeSpawningManager::InitializePatterns()
{
    TreePatterns.Add(FDaytimeSpawnPattern(
        TEXT("Pine Tree Growth"),
        ESeason::Spring,
        6.f, // 6 AM
        18.f, // 6 PM
        TEXT("Pine trees grow rapidly during spring daytime hours.")
    ));

    BerryPatterns.Add(FDaytimeSpawnPattern(
        TEXT("Blueberry Ripening"),
        ESeason::Summer,
        8.f, // 8 AM
        16.f, // 4 PM
        TEXT("Blueberries ripen during summer days.")
    ));

    FlowerPatterns.Add(FDaytimeSpawnPattern(
        TEXT("Sunflower Bloom"),
        ESeason::Summer,
        6.f, // 6 AM
        12.f, // 12 PM
        TEXT("Sunflowers bloom with the rising sun.")
    ));

    CreaturePatterns.Add(FDaytimeSpawnPattern(
        TEXT("Deer Grazing"),
        ESeason::Autumn,
        9.f, // 9 AM
        17.f, // 5 PM
        TEXT("Deer are active during autumn days, grazing in open fields.")
    ));
}

void ADaytimeSpawningManager::SimulateTime(float DeltaSeconds)
{
    // Increment current hour (1 second = 1 in-game minute)
    CurrentHour += DeltaSeconds / 60.f;

    // Wrap around the hour if it exceeds 24
    if (CurrentHour >= 24.f)
    {
        CurrentHour -= 24.f;
    }
}

void ADaytimeSpawningManager::CheckActivePatterns(const TArray<FDaytimeSpawnPattern>& Patterns, const FString& Category) const
{
    UE_LOG(LogTemp, Log, TEXT("--- Checking %s ---"), *Category);
    for (const FDaytimeSpawnPattern& Pattern : Patterns)
    {
        if (Pattern.IsActive(CurrentHour, CurrentSeason))
        {
            UE_LOG(LogTemp, Log, TEXT("Active Pattern: %s - %s"), *Pattern.EntityName, *Pattern.Description);
        }
    }
}

