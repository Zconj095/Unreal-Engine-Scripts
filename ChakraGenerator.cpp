#include "ChakraGenerator.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AChakraGenerator::AChakraGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    // Default sphere mesh: Engine Basic Sphere
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereFinder.Succeeded())
    {
        SphereMesh = SphereFinder.Object;
    }
}

void AChakraGenerator::BeginPlay()
{
    Super::BeginPlay();

    if (ParentActor)
    {
        AttachToActor(ParentActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        SetActorRelativeLocation(FVector::ZeroVector);
        SetActorRelativeRotation(FRotator::ZeroRotator);
    }

    CreateChakras();
    CreateAura();
}

void AChakraGenerator::CreateChakras()
{
    if (!SphereMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("ChakraGenerator: SphereMesh not set."));
        return;
    }

    const TArray<FLinearColor> ChakraColors = {
        FLinearColor(1.f, 0.f, 0.f),        // Root - Red
        FLinearColor(1.f, 0.6f, 0.f),       // Sacral - Orange
        FLinearColor(1.f, 1.f, 0.f),        // Solar Plexus - Yellow
        FLinearColor(0.f, 1.f, 0.f),        // Heart - Green
        FLinearColor(0.f, 1.f, 1.f),        // Throat - Cyan
        FLinearColor(0.5f, 0.f, 1.f),       // Third Eye - Indigo/Violet-ish
        FLinearColor(0.8f, 0.4f, 1.f)       // Crown - Violet
    };

    const TArray<FString> ChakraNames = {
        TEXT("Root Chakra"),
        TEXT("Sacral Chakra"),
        TEXT("Solar Plexus Chakra"),
        TEXT("Heart Chakra"),
        TEXT("Throat Chakra"),
        TEXT("Third Eye Chakra"),
        TEXT("Crown Chakra")
    };

    const int32 Count = ChakraColors.Num();
    ChakraComponents.Reserve(Count);

    for (int32 i = 0; i < Count; ++i)
    {
        UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("%s"), *ChakraNames[i]));
        if (!Comp)
        {
            continue;
        }

        Comp->SetupAttachment(Root);
        Comp->SetStaticMesh(SphereMesh);
        Comp->SetMobility(EComponentMobility::Movable);
        Comp->RegisterComponent();

        // Position along vertical (Unreal Z axis) at ChakraSpacing intervals
        Comp->SetRelativeLocation(FVector(0.f, 0.f, i * ChakraSpacing));

        // Color via dynamic material if available
        if (ChakraBaseMaterial)
        {
            if (UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(ChakraBaseMaterial, this))
            {
                MID->SetVectorParameterValue(ColorParamName, ChakraColors[i]);
                Comp->SetMaterial(0, MID);
            }
        }
        else
        {
            UE_LOG(LogTemp, Verbose, TEXT("ChakraGenerator: ChakraBaseMaterial not set; using mesh default for %s."), *ChakraNames[i]);
        }

        ChakraComponents.Add(Comp);
    }
}

void AChakraGenerator::CreateAura()
{
    if (!SphereMesh)
    {
        return;
    }

    AuraComponent = NewObject<UStaticMeshComponent>(this, TEXT("Energy Aura"));
    if (!AuraComponent)
    {
        return;
    }

    AuraComponent->SetupAttachment(Root);
    AuraComponent->SetStaticMesh(SphereMesh);
    AuraComponent->SetMobility(EComponentMobility::Movable);
    AuraComponent->RegisterComponent();

    AuraComponent->SetRelativeLocation(FVector::ZeroVector);
    AuraComponent->SetRelativeScale3D(FVector(AuraScaleFactor));

    if (AuraTranslucentMaterial)
    {
        if (UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(AuraTranslucentMaterial, this))
        {
            // Neutral white color with opacity
            MID->SetVectorParameterValue(ColorParamName, FLinearColor::White);
            MID->SetScalarParameterValue(OpacityParamName, FMath::Clamp(AuraTransparency, 0.0f, 1.0f));
            AuraComponent->SetMaterial(0, MID);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ChakraGenerator: AuraTranslucentMaterial not set; aura will not be transparent."));
    }
}

