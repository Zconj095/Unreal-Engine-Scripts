#include "PrecisionCalibrationComponent.h"
#include "Logging/LogMacros.h"

UPrecisionCalibrationComponent::UPrecisionCalibrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPrecisionCalibrationComponent::BeginPlay()
{
	Super::BeginPlay();

	Adjustment = CalculateAdjustment();
	if (Adjustment.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Adjustment Magnitude: %s"), *FString::JoinBy(Adjustment, TEXT(", "), [](float Value) { return FString::SanitizeFloat(Value); }));
	}
}

TArray<float> UPrecisionCalibrationComponent::CalculateAdjustment() const
{
	if (MatrixSize <= 0 || CalibrationMatrix.Num() != MatrixSize * MatrixSize || DeltaX.Num() != MatrixSize)
	{
		UE_LOG(LogTemp, Error, TEXT("Calibration matrix dimensions or delta vector size mismatch."));
		return TArray<float>();
	}

	TArray<float> Result;
	Result.Init(0.0f, MatrixSize);

	for (int32 Row = 0; Row < MatrixSize; ++Row)
	{
		float Sum = 0.0f;
		for (int32 Col = 0; Col < MatrixSize; ++Col)
		{
			const int32 Index = Row * MatrixSize + Col;
			Sum += CalibrationMatrix[Index] * DeltaX[Col];
		}
		Result[Row] = Sum;
	}

	return Result;
}
