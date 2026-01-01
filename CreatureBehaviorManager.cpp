#include "CreatureBehaviorManager.h"

ACreatureBehaviorManager::ACreatureBehaviorManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACreatureBehaviorManager::BeginPlay()
{
    Super::BeginPlay();

    // Define example creature patterns
    CreaturePatterns.Add(FCreaturePattern(
        TEXT("Night Owl"),
        EDayPhase::Nighttime,
        ESeason::Winter,
        20.f,  // 8 PM
        4.f,   // 4 AM
        TEXT("Forages for food under the cover of darkness.")
    ));

    CreaturePatterns.Add(FCreaturePattern(
        TEXT("Sunbird"),
        EDayPhase::Daytime,
        ESeason::Summer,
        6.f,   // 6 AM
        18.f,  // 6 PM
        TEXT("Active during the bright summer days, searching for nectar.")
    ));

    CreaturePatterns.Add(FCreaturePattern(
        TEXT("Twilight Fox"),
        EDayPhase::Nighttime,
        ESeason::Autumn,
        18.f,  // 6 PM
        6.f,   // 6 AM
        TEXT("Hunts small prey and hides during the day.")
    ));

    // Display all patterns
    for (const FCreaturePattern& Pattern : CreaturePatterns)
    {
        Pattern.DisplayDetails();
    }
}

void ACreatureBehaviorManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Check active patterns every frame (or adapt to game tick)
    const UEnum* DayPhaseEnum = StaticEnum<EDayPhase>();
    const UEnum* SeasonEnum = StaticEnum<ESeason>();
    const FString PhaseStr = DayPhaseEnum ? DayPhaseEnum->GetNameStringByValue(static_cast<int64>(CurrentPhase)) : TEXT("UnknownPhase");
    const FString SeasonStr = SeasonEnum ? SeasonEnum->GetNameStringByValue(static_cast<int64>(CurrentSeason)) : TEXT("UnknownSeason");

    UE_LOG(LogTemp, Log, TEXT("Current Simulation Time: %.0f:00, Phase: %s, Season: %s"), CurrentHour, *PhaseStr, *SeasonStr);

    for (const FCreaturePattern& Pattern : CreaturePatterns)
    {
        if (Pattern.IsActive(CurrentHour, CurrentSeason, CurrentPhase))
        {
            UE_LOG(LogTemp, Log, TEXT("Active Pattern: %s - %s"), *Pattern.CreatureName, *Pattern.Behavior);
        }
    }

    // Simulate time progression
    SimulateTime(DeltaSeconds);
}

void ACreatureBehaviorManager::SimulateTime(float DeltaSeconds)
{
    // Simulate minutes passing (1 real second = 1 in-game minute)
    CurrentHour += DeltaSeconds / 60.f;
    if (CurrentHour >= 24.f)
    {
        CurrentHour -= 24.f;
        UpdatePhase();
    }
}

void ACreatureBehaviorManager::UpdatePhase()
{
    if (CurrentHour >= 6.f && CurrentHour < 18.f)
    {
        CurrentPhase = EDayPhase::Daytime;
    }
    else
    {
        CurrentPhase = EDayPhase::Nighttime;
    }
}

