#include "IndependentAuricLayerGenerator.h"

#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AIndependentAuricLayerGenerator::AIndependentAuricLayerGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    // Default sphere mesh: Engine Basic Sphere
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereFinder.Succeeded())
    {
        SphereMesh = SphereFinder.Object;
    }
}

void AIndependentAuricLayerGenerator::BeginPlay()
{
    Super::BeginPlay();
    GenerateAuricLayers();
}

void AIndependentAuricLayerGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ClearExisting();
    Super::EndPlay(EndPlayReason);
}

void AIndependentAuricLayerGenerator::GenerateAuricLayers()
{
    ClearExisting();

    const int32 Expected = AuricLayerNames.Num();
    if (AuricLayerColors.Num() != Expected)
    {
        UE_LOG(LogTemp, Warning, TEXT("IndependentAuricLayerGenerator: Colors count (%d) does not match Names count (%d)."), AuricLayerColors.Num(), Expected);
    }
    if (AuraParentActors.Num() < Expected)
    {
        UE_LOG(LogTemp, Warning, TEXT("IndependentAuricLayerGenerator: Not all parent actors assigned (%d/%d)."), AuraParentActors.Num(), Expected);
    }

    LayerActors.SetNum(Expected);
    for (int32 i = 0; i < Expected; ++i)
    {
        if (!AuraParentActors.IsValidIndex(i) || !IsValid(AuraParentActors[i]))
        {
            UE_LOG(LogTemp, Error, TEXT("IndependentAuricLayerGenerator: Missing parent for %s (index %d)."), *AuricLayerNames[i], i);
            continue;
        }
        CreateAuricLayer(i);
    }
}

void AIndependentAuricLayerGenerator::ClearExisting()
{
    for (AStaticMeshActor* Actor : LayerActors)
    {
        if (IsValid(Actor))
        {
            Actor->Destroy();
        }
    }
    LayerActors.Empty();
}

void AIndependentAuricLayerGenerator::CreateAuricLayer(int32 Index)
{
    if (!GetWorld() || !SphereMesh)
    {
        return;
    }

    AActor* Parent = AuraParentActors[Index];
    const FString& Name = AuricLayerNames.IsValidIndex(Index) ? AuricLayerNames[Index] : FString::Printf(TEXT("Auric Layer %d"), Index);
    const FLinearColor Color = AuricLayerColors.IsValidIndex(Index) ? AuricLayerColors[Index] : FLinearColor::White;

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Name = FName(*Name);
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawn at parent's location; will attach and zero relative after
    const FVector SpawnLoc = Parent->GetActorLocation();
    AStaticMeshActor* SphereActor = GetWorld()->SpawnActor<AStaticMeshActor>(SpawnLoc, FRotator::ZeroRotator, Params);
    if (!SphereActor)
    {
        return;
    }

    SphereActor->SetMobility(EComponentMobility::Movable);
    UStaticMeshComponent* MeshComp = SphereActor->GetStaticMeshComponent();
    if (MeshComp)
    {
        MeshComp->SetStaticMesh(SphereMesh);

        if (TranslucentMaterial)
        {
            if (UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(TranslucentMaterial, this))
            {
                MID->SetVectorParameterValue(ColorParamName, Color);
                MID->SetScalarParameterValue(OpacityParamName, FMath::Clamp(AuraTransparency, 0.0f, 1.0f));
                MeshComp->SetMaterial(0, MID);
            }
        }
    }

    // Attach to parent actor and set relative transform
    SphereActor->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform);
    SphereActor->SetActorRelativeLocation(FVector::ZeroVector);

    const float Scale = BaseScale + (Index * ScaleIncrement);
    SphereActor->SetActorRelativeScale3D(FVector(Scale));

    LayerActors[Index] = SphereActor;
}

