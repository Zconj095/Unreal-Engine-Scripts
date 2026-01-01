// Copyright Epic Games, Inc.

#include "CircuitExecutorActor.h"

#include "GroverCircuitComponent.h"
#include "HadamardCircuitComponent.h"
#include "PauliLindbladCircuitComponent.h"
#include "RadonCircuitComponent.h"
#include "ShorCircuitComponent.h"
#include "Components/SceneComponent.h"
#include "Math/UnrealMathUtility.h"

ACircuitExecutorActor::ACircuitExecutorActor()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    HadamardCircuit = CreateDefaultSubobject<UHadamardCircuitComponent>(TEXT("HadamardCircuit"));
    ShorCircuit = CreateDefaultSubobject<UShorCircuitComponent>(TEXT("ShorCircuit"));
    GroverCircuit = CreateDefaultSubobject<UGroverCircuitComponent>(TEXT("GroverCircuit"));
    RadonCircuit = CreateDefaultSubobject<URadonCircuitComponent>(TEXT("RadonCircuit"));
    PauliLindbladCircuit = CreateDefaultSubobject<UPauliLindbladCircuitComponent>(TEXT("PauliLindbladCircuit"));
}

void ACircuitExecutorActor::BeginPlay()
{
    Super::BeginPlay();

    if (HadamardCircuit)
    {
        HadamardCircuit->ApplyHadamard(NumQubits);
    }

    if (ShorCircuit)
    {
        ShorCircuit->ExecuteShor(15);
    }

    if (GroverCircuit)
    {
        GroverCircuit->ExecuteGrover(NumQubits, 2);
    }

    if (RadonCircuit)
    {
        FRadonMatrix2x2 Matrix;
        Matrix.M00 = 1.0f; Matrix.M01 = 0.0f;
        Matrix.M10 = 0.0f; Matrix.M11 = 1.0f;
        RadonCircuit->ApplyRadonTransform(Matrix, PI / 4.0f);
    }

    if (PauliLindbladCircuit)
    {
        PauliLindbladCircuit->ApplyPauliLindblad(FVector(1.0f, 0.0f, 0.0f));
    }
}
