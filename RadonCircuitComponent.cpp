// Copyright Epic Games, Inc.

#include "RadonCircuitComponent.h"
#include "Math/UnrealMathUtility.h"

URadonCircuitComponent::URadonCircuitComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URadonCircuitComponent::ApplyRadonTransform(const FRadonMatrix2x2& Matrix, float AngleRadians)
{
    UE_LOG(LogTemp, Log, TEXT("Applying Radon transform with matrix [[%f, %f], [%f, %f]] at angle %f radians."),
        Matrix.M00, Matrix.M01, Matrix.M10, Matrix.M11, AngleRadians);
}
