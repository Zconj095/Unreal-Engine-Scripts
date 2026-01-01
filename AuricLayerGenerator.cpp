#include "AuricLayerGenerator.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AAuricLayerGenerator::AAuricLayerGenerator()
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

void AAuricLayerGenerator::BeginPlay()
{
    Super::BeginPlay();

    if (ParentActor)
    {
        AttachToActor(ParentActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        SetActorRelativeLocation(FVector::ZeroVector);
        SetActorRelativeRotation(FRotator::ZeroRotator);
    }

    GenerateAuricLayers();
}

void AAuricLayerGenerator::GenerateAuricLayers()
{
    // Names and colors roughly matching the Unity script
    const TArray<FString> Names = {
        TEXT("Etheric Body"),
        TEXT("Emotional Body"),
        TEXT("Mental Body"),
        TEXT("Astral Body"),
        TEXT("Etheric Template"),
        TEXT("Celestial Body"),
        TEXT("Causal Body")
    };

    const TArray<FLinearColor> Colors = {
        FLinearColor(0.9f, 0.9f, 1.0f), // light blue
        FLinearColor(1.0f, 0.7f, 0.7f), // light red/pink
        FLinearColor(1.0f, 1.0f, 0.6f), // light yellow
        FLinearColor(0.6f, 1.0f, 0.6f), // light green
        FLinearColor(0.7f, 0.7f, 1.0f), // light purple/blue
        FLinearColor(0.9f, 0.6f, 1.0f), // light purple
        FLinearColor(1.0f, 1.0f, 1.0f)  // white
    };

    const int32 Count = Names.Num();
    LayerComponents.Reserve(Count);

    for (int32 i = 0; i < Count; ++i)
    {
        CreateAuricLayer(i, Names[i], Colors[i]);
    }
}

void AAuricLayerGenerator::CreateAuricLayer(int32 Index, const FString& Name, const FLinearColor& Color)
{
    if (!SphereMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("AuricLayerGenerator: SphereMesh not set."));
        return;
    }

    UStaticMeshComponent* Layer = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("%s_Component_%d"), *Name, Index));
    if (!Layer)
    {
        return;
    }

    Layer->SetupAttachment(Root);
    Layer->SetStaticMesh(SphereMesh);
    Layer->SetMobility(EComponentMobility::Movable);
    Layer->RegisterComponent();

    // Center at the actor origin
    Layer->SetRelativeLocation(FVector::ZeroVector);

    // Progressive uniform scale
    const float Scale = BaseScale + (Index * ScaleIncrement);
    Layer->SetRelativeScale3D(FVector(Scale));

    // Apply translucent material via dynamic instance
    if (TranslucentMaterial)
    {
        UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(TranslucentMaterial, this);
        if (MID)
        {
            MID->SetVectorParameterValue(ColorParamName, Color);
            MID->SetScalarParameterValue(OpacityParamName, FMath::Clamp(AuraTransparency, 0.0f, 1.0f));
            Layer->SetMaterial(0, MID);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AuricLayerGenerator: TranslucentMaterial is not set. Please assign a translucent material with %s (Vector) and %s (Scalar) parameters."), *ColorParamName.ToString(), *OpacityParamName.ToString());
    }

    LayerComponents.Add(Layer);
}

