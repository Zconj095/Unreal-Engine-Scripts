#include "EnviroEnvironment.h"

AEnviroEnvironment::AEnviroEnvironment()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnviroEnvironment::BeginPlay()
{
    Super::BeginPlay();

    // Creating sample biomes
    FBiome Forest(
        TEXT("Forest"),
        TArray<FString>{ TEXT("Oak Trees"), TEXT("Pine Trees"), TEXT("Shrubs") },
        TArray<FString>{ TEXT("Deer"), TEXT("Birds"), TEXT("Wolves") },
        TEXT("A region covered with dense trees and diverse wildlife.")
    );

    FBiome Desert(
        TEXT("Desert"),
        TArray<FString>{ TEXT("Cacti"), TEXT("Succulents") },
        TArray<FString>{ TEXT("Snakes"), TEXT("Lizards"), TEXT("Scorpions") },
        TEXT("A dry, arid region with minimal vegetation.")
    );

    // Creating sample habitats
    FHabitat ForestHabitat(
        TEXT("Dense Forest Habitat"),
        TEXT("Forest"),
        TEXT("Water sources, Dense vegetation, Moderate temperature"),
        TEXT("Supports diverse life forms due to optimal conditions.")
    );

    FHabitat DesertHabitat(
        TEXT("Desert Habitat"),
        TEXT("Desert"),
        TEXT("Limited water, Extreme temperature adaptations"),
        TEXT("Supports specialized life forms adapted to arid conditions.")
    );

    // Creating a sample ecosystem
    FEcosystem MixedEcosystem(
        TEXT("Mixed Terrain Ecosystem"),
        TArray<FBiome>{ Forest, Desert },
        TEXT("Seasonal rain and dry spells"),
        TEXT("Varied terrain including rocky areas, sandy plains, and dense forests"),
        TEXT("A combination of biomes with unique weather and terrain impacts.")
    );

    // Display details
    Forest.DisplayDetails();
    Desert.DisplayDetails();
    ForestHabitat.DisplayDetails();
    DesertHabitat.DisplayDetails();
    MixedEcosystem.DisplayDetails();
}

