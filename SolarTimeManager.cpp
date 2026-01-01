#include "SolarTimeManager.h"

ASolarTimeManager::ASolarTimeManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASolarTimeManager::BeginPlay()
{
    Super::BeginPlay();

    // Initialize Timeframes
    InitializeTimeframes();

    // Initialize Twilight Colors
    InitializeTwilightColors();

    // Display current active timeframe
    UpdateTimeframe();
}

void ASolarTimeManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Simulate time progression
    SimulateTime(DeltaSeconds);

    // Update and display the current timeframe
    UpdateTimeframe();
}

void ASolarTimeManager::InitializeTimeframes()
{
    SolarTimeframes.Add(FSolarTimeframe(ETimeframe::Twilight,   5.f,  6.f,  TEXT("Hours between daybreak and sunrise.")));
    SolarTimeframes.Add(FSolarTimeframe(ETimeframe::Sunrise,    6.f,  8.f,  TEXT("Early morning hours of sunrise.")));
    SolarTimeframes.Add(FSolarTimeframe(ETimeframe::Morning,    8.f,  12.f, TEXT("The time after sunrise and before noon.")));
    SolarTimeframes.Add(FSolarTimeframe(ETimeframe::Noon,       12.f, 13.f, TEXT("The midpoint of the day.")));
    SolarTimeframes.Add(FSolarTimeframe(ETimeframe::Afternoon,  13.f, 17.f, TEXT("The time after morning and before evening.")));
    SolarTimeframes.Add(FSolarTimeframe(ETimeframe::Evening,    17.f, 20.f, TEXT("The hours leading into nighttime.")));
    SolarTimeframes.Add(FSolarTimeframe(ETimeframe::Midnight,   0.f,  5.f,  TEXT("The end of the day and beginning of night.")));
}

void ASolarTimeManager::InitializeTwilightColors()
{
    TwilightColors.Add(FTwilightColor(TEXT("Dawn"), FLinearColor(1.f, 0.7f, 0.5f), TEXT("The warm colors of dawn during twilight.")));
    TwilightColors.Add(FTwilightColor(TEXT("Dusk"), FLinearColor(0.8f, 0.5f, 0.7f), TEXT("The cool colors of dusk during twilight.")));
}

void ASolarTimeManager::SimulateTime(float DeltaSeconds)
{
    // Increment time (simulate time passing in hours)
    CurrentHour += DeltaSeconds / 60.f; // 1 real-time second = 1 in-game minute
    if (CurrentHour >= 24.f)
    {
        CurrentHour -= 24.f; // Wrap around to 0 after 24 hours
    }
}

void ASolarTimeManager::UpdateTimeframe()
{
    for (const FSolarTimeframe& Timeframe : SolarTimeframes)
    {
        if (Timeframe.IsActive(CurrentHour))
        {
            if (CurrentActiveTimeframe != Timeframe.Name)
            {
                CurrentActiveTimeframe = Timeframe.Name;
                const UEnum* TFEnum = StaticEnum<ETimeframe>();
                const FString NameStr = TFEnum ? TFEnum->GetNameStringByValue(static_cast<int64>(CurrentActiveTimeframe)) : TEXT("Unknown");
                UE_LOG(LogTemp, Log, TEXT("Current Timeframe: %s - %s"), *NameStr, *Timeframe.Description);
            }
        }
    }

    // Update twilight colors if in Twilight timeframe
    if (CurrentActiveTimeframe == ETimeframe::Twilight)
    {
        UpdateTwilightColor();
    }
}

void ASolarTimeManager::UpdateTwilightColor()
{
    for (const FTwilightColor& TwColor : TwilightColors)
    {
        UE_LOG(LogTemp, Log, TEXT("Twilight Color: %s - %s"), *TwColor.Name, *TwColor.Description);
        // In Unreal, environment lighting is handled via actors/components (e.g., SkyLight/DirectionalLight).
        // Here we only log; apply color in your lighting system if desired.
    }
}

