#include "RotationMatrixPredictorComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

URotationMatrixPredictorComponent::URotationMatrixPredictorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URotationMatrixPredictorComponent::BeginPlay()
{
	Super::BeginPlay();
	SimulatePredictions({ InputVector });
	if (bLogOnBeginPlay)
	{
		const FVector Output = PredictTransformedVector(InputVector, RotationAngleDegrees, RotationAxis);
		UE_LOG(LogTemp, Log, TEXT("Predicted transformed vector: X=%.4f Y=%.4f Z=%.4f"), Output.X, Output.Y, Output.Z);
	}
}

FVector URotationMatrixPredictorComponent::PredictTransformedVector(const FVector& VectorToRotate, float AngleDegrees, const FVector& Axis) const
{
	const FVector NormalAxis = Axis.GetSafeNormal();
	if (!NormalAxis.IsNearlyZero())
	{
		const float Radians = FMath::DegreesToRadians(AngleDegrees);
		const FQuat RotationQuat = FQuat(NormalAxis, Radians);
		return RotationQuat.RotateVector(VectorToRotate);
	}
	UE_LOG(LogTemp, Warning, TEXT("Rotation axis is nearly zero, returning original vector."));
	return VectorToRotate;
}

void URotationMatrixPredictorComponent::SimulatePredictions(const TArray<FVector>& Vectors)
{
	SampleResults.Empty();
	for (int32 Index = 0; Index < Vectors.Num(); ++Index)
	{
		const FVector Result = PredictTransformedVector(Vectors[Index], RotationAngleDegrees, RotationAxis);
		SampleResults.Add(Result);
		UE_LOG(LogTemp, Log, TEXT("Sample %d => (%.3f, %.3f, %.3f)"), Index, Result.X, Result.Y, Result.Z);
	}
}
