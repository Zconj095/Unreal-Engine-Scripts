#include "ChakraAndBrainwaveGenerator.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AChakraAndBrainwaveGenerator::AChakraAndBrainwaveGenerator()
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

    // Brainwave classes and ranges (Hz)
    BrainwaveClasses = { TEXT("Delta"), TEXT("Theta"), TEXT("Alpha"), TEXT("Beta"), TEXT("Gamma") };
    BrainwaveRanges = {
        FVector2D(0.5f, 4.0f),
        FVector2D(4.0f, 8.0f),
        FVector2D(8.0f, 12.0f),
        FVector2D(12.0f, 30.0f),
        FVector2D(30.0f, 100.0f)
    };
}

void AChakraAndBrainwaveGenerator::BeginPlay()
{
    Super::BeginPlay();

    if (BrainwaveParentActor)
    {
        AttachToActor(BrainwaveParentActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        SetActorRelativeLocation(FVector::ZeroVector);
        SetActorRelativeRotation(FRotator::ZeroRotator);
    }

    GenerateBrainwaves();
}

void AChakraAndBrainwaveGenerator::GenerateBrainwaves()
{
    if (!SphereMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("ChakraAndBrainwaveGenerator: SphereMesh not set."));
        return;
    }

    const float OverallMin = BrainwaveRanges.Num() > 0 ? BrainwaveRanges[0].X : 0.0f;
    const float OverallMax = BrainwaveRanges.Num() > 0 ? BrainwaveRanges.Last().Y : 1.0f;

    BrainwaveComponents.Reserve(NumBrainwaves);
    for (int32 i = 0; i < NumBrainwaves; ++i)
    {
        const int32 ClassIndex = FMath::RandRange(0, BrainwaveClasses.Num() - 1);
        const FVector2D Range = BrainwaveRanges.IsValidIndex(ClassIndex) ? BrainwaveRanges[ClassIndex] : FVector2D(0.f, 1.f);

        float Frequency = FMath::FRandRange(Range.X, Range.Y);
        Frequency = FMath::RoundToFloat(Frequency * 10000.f) / 10000.f; // 4 decimal places

        CreateBrainwave(BrainwaveClasses[ClassIndex], ClassIndex, Frequency, OverallMin, OverallMax);
    }
}

void AChakraAndBrainwaveGenerator::CreateBrainwave(const FString& ClassName, int32 ClassIndex, float Frequency, float OverallMin, float OverallMax)
{
    UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("%s_%.4fHz"), *ClassName, Frequency));
    if (!Comp)
    {
        return;
    }

    Comp->SetupAttachment(Root);
    Comp->SetStaticMesh(SphereMesh);
    Comp->SetMobility(EComponentMobility::Movable);
    Comp->RegisterComponent();

    // Random local position within cube
    const float R = PositionRange;
    const FVector LocalPos(FMath::FRandRange(-R, R), FMath::FRandRange(-R, R), FMath::FRandRange(-R, R));
    Comp->SetRelativeLocation(LocalPos);

    // Scale based on normalized frequency across global min..max
    const float T = (OverallMax > OverallMin) ? FMath::Clamp((Frequency - OverallMin) / (OverallMax - OverallMin), 0.0f, 1.0f) : 0.0f;
    const float Scale = FMath::Lerp(MinBrainwaveSize, MaxBrainwaveSize, T);
    Comp->SetRelativeScale3D(FVector(Scale));

    // Color based on class
    const FLinearColor Color = ClassIndexToColor(ClassIndex);
    if (BaseMaterial)
    {
        if (UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMaterial, this))
        {
            MID->SetVectorParameterValue(ColorParamName, Color);
            Comp->SetMaterial(0, MID);
        }
    }
    else
    {
        UE_LOG(LogTemp, Verbose, TEXT("ChakraAndBrainwaveGenerator: BaseMaterial not set; spheres will use mesh default."));
    }

    BrainwaveComponents.Add(Comp);
}

FLinearColor AChakraAndBrainwaveGenerator::ClassIndexToColor(int32 ClassIndex) const
{
    switch (ClassIndex)
    {
    case 0: return FLinearColor(0.0f, 0.0f, 1.0f); // Delta - Blue
    case 1: return FLinearColor(1.0f, 0.0f, 1.0f); // Theta - Magenta
    case 2: return FLinearColor(0.0f, 1.0f, 0.0f); // Alpha - Green
    case 3: return FLinearColor(1.0f, 1.0f, 0.0f); // Beta  - Yellow
    case 4: return FLinearColor(1.0f, 0.0f, 0.0f); // Gamma - Red
    default: return FLinearColor::White;
    }
}

