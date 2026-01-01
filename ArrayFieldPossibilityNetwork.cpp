#include "ArrayFieldPossibilityNetwork.h"

#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AArrayFieldPossibilityNetwork::AArrayFieldPossibilityNetwork()
{
    PrimaryActorTick.bCanEverTick = false;

    // Provide a sensible default cube mesh (Engine basic cube)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMeshFinder.Succeeded())
    {
        CubeMesh = CubeMeshFinder.Object;
    }
}

void AArrayFieldPossibilityNetwork::BeginPlay()
{
    Super::BeginPlay();

    const int64 Total = static_cast<int64>(DimensionX) * DimensionY * DimensionZ;
    if (Total <= 0)
    {
        return;
    }

    PossibilityField.SetNum(Total);
    CubeField.SetNum(Total);

    InitializeField();
    CreateVisualization();

    if (EvolveInterval > 0.f)
    {
        GetWorldTimerManager().SetTimer(
            EvolveTimerHandle,
            this,
            &AArrayFieldPossibilityNetwork::EvolveField,
            EvolveInterval,
            true
        );
    }
}

void AArrayFieldPossibilityNetwork::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(EvolveTimerHandle);

    // Clean up spawned actors (optional)
    for (AStaticMeshActor* Actor : CubeField)
    {
        if (IsValid(Actor))
        {
            Actor->Destroy();
        }
    }
    CubeField.Empty();

    Super::EndPlay(EndPlayReason);
}

void AArrayFieldPossibilityNetwork::InitializeField()
{
    for (int32 x = 0; x < DimensionX; ++x)
    {
        for (int32 y = 0; y < DimensionY; ++y)
        {
            for (int32 z = 0; z < DimensionZ; ++z)
            {
                const int32 idx = Index(x, y, z);
                PossibilityField[idx] = FMath::FRandRange(0.0f, 1.0f);
            }
        }
    }
}

void AArrayFieldPossibilityNetwork::CreateVisualization()
{
    if (!GetWorld())
    {
        return;
    }

    if (!CubeMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("ArrayFieldPossibilityNetwork: CubeMesh is not set; using default if available."));
    }

    for (int32 x = 0; x < DimensionX; ++x)
    {
        for (int32 y = 0; y < DimensionY; ++y)
        {
            for (int32 z = 0; z < DimensionZ; ++z)
            {
                const int32 idx = Index(x, y, z);
                const FVector Location = GetActorLocation() + FVector(x * Spacing, y * Spacing, z * Spacing);

                FActorSpawnParameters Params;
                Params.Owner = this;
                Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                AStaticMeshActor* CubeActor = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator, Params);
                if (CubeActor)
                {
                    CubeActor->SetMobility(EComponentMobility::Movable);
                    UStaticMeshComponent* MeshComp = CubeActor->GetStaticMeshComponent();
                    if (MeshComp)
                    {
                        if (CubeMesh)
                        {
                            MeshComp->SetStaticMesh(CubeMesh);
                        }
                        MeshComp->SetWorldScale3D(FVector((float)PossibilityField[idx]));
                    }
                }

                CubeField[idx] = CubeActor;
            }
        }
    }
}

void AArrayFieldPossibilityNetwork::UpdateVisualization()
{
    for (int32 x = 0; x < DimensionX; ++x)
    {
        for (int32 y = 0; y < DimensionY; ++y)
        {
            for (int32 z = 0; z < DimensionZ; ++z)
            {
                const int32 idx = Index(x, y, z);
                AStaticMeshActor* CubeActor = CubeField.IsValidIndex(idx) ? CubeField[idx] : nullptr;
                if (IsValid(CubeActor))
                {
                    CubeActor->SetActorScale3D(FVector((float)PossibilityField[idx]));
                }
            }
        }
    }
}

void AArrayFieldPossibilityNetwork::EvolveField()
{
    TArray<double> NewField;
    NewField.SetNum(PossibilityField.Num());

    for (int32 x = 0; x < DimensionX; ++x)
    {
        for (int32 y = 0; y < DimensionY; ++y)
        {
            for (int32 z = 0; z < DimensionZ; ++z)
            {
                const int32 idx = Index(x, y, z);
                NewField[idx] = QuantumEvolutionRule(x, y, z);
            }
        }
    }

    PossibilityField = MoveTemp(NewField);
    UpdateVisualization();
}

double AArrayFieldPossibilityNetwork::QuantumEvolutionRule(int32 x, int32 y, int32 z) const
{
    const int32 idx = Index(x, y, z);
    double sum = PossibilityField[idx];
    int32 neighbors = 0;

    for (int32 i = -1; i <= 1; ++i)
    {
        for (int32 j = -1; j <= 1; ++j)
        {
            for (int32 k = -1; k <= 1; ++k)
            {
                if (i == 0 && j == 0 && k == 0) continue;

                const int32 nx = x + i;
                const int32 ny = y + j;
                const int32 nz = z + k;

                if (nx >= 0 && nx < DimensionX && ny >= 0 && ny < DimensionY && nz >= 0 && nz < DimensionZ)
                {
                    sum += PossibilityField[Index(nx, ny, nz)];
                    neighbors++;
                }
            }
        }
    }

    return sum / double(neighbors + 1);
}

