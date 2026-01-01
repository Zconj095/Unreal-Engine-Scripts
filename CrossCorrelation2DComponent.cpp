#include "CrossCorrelation2DComponent.h"

#include "Logging/LogMacros.h"

UCrossCorrelation2DComponent::UCrossCorrelation2DComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Default data matching the Unity example
	Image = {
		FFloatRow{{1.f, 2.f, 3.f}},
		FFloatRow{{4.f, 5.f, 6.f}},
		FFloatRow{{7.f, 8.f, 9.f}}
	};

	Kernel = {
		FFloatRow{{1.f, 0.f}},
		FFloatRow{{0.f, -1.f}}
	};
}

void UCrossCorrelation2DComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FFloatRow> Result;
	ComputeCrossCorrelation2D(Image, Kernel, Result);

	if (Result.Num() == 0)
	{
		// Errors already logged by ComputeCrossCorrelation2D
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("2D Cross Correlation Result:"));
	for (const FFloatRow& Row : Result)
	{
		FString RowStr;
		for (int32 j = 0; j < Row.Values.Num(); ++j)
		{
			RowStr += FString::Printf(TEXT("%.2f"), Row.Values[j]);
			if (j + 1 < Row.Values.Num())
			{
				RowStr += TEXT(" ");
			}
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), *RowStr);
	}
}

void UCrossCorrelation2DComponent::ComputeCrossCorrelation2D(const TArray<FFloatRow>& InImage, const TArray<FFloatRow>& InKernel, TArray<FFloatRow>& OutResult) const
{
	OutResult.Reset();

	TArray<TArray<float>> ImageMat;
	TArray<TArray<float>> KernelMat;
	int32 ImageRows = 0, ImageCols = 0;
	int32 KernelRows = 0, KernelCols = 0;

	if (!ToMatrix(InImage, ImageMat, ImageRows, ImageCols) || !ToMatrix(InKernel, KernelMat, KernelRows, KernelCols))
	{
		UE_LOG(LogTemp, Error, TEXT("CrossCorrelation2DComponent: Image or Kernel is empty or non-rectangular."));
		return;
	}

	if (KernelRows > ImageRows || KernelCols > ImageCols)
	{
		UE_LOG(LogTemp, Error, TEXT("CrossCorrelation2DComponent: Kernel dimensions are larger than the image dimensions. Cross-correlation not possible."));
		return;
	}

	const int32 OutputRows = ImageRows - KernelRows + 1;
	const int32 OutputCols = ImageCols - KernelCols + 1;

	TArray<TArray<float>> Correlation;
	Correlation.SetNum(OutputRows);
	for (int32 r = 0; r < OutputRows; ++r)
	{
		Correlation[r].SetNumZeroed(OutputCols);
	}

	for (int32 i = 0; i < OutputRows; ++i)
	{
		for (int32 j = 0; j < OutputCols; ++j)
		{
			float Sum = 0.f;
			for (int32 m = 0; m < KernelRows; ++m)
			{
				for (int32 n = 0; n < KernelCols; ++n)
				{
					Sum += ImageMat[i + m][j + n] * KernelMat[m][n];
				}
			}
			Correlation[i][j] = Sum;
		}
	}

	MatrixToRows(Correlation, OutResult);
}

bool UCrossCorrelation2DComponent::ToMatrix(const TArray<FFloatRow>& Rows, TArray<TArray<float>>& OutMatrix, int32& OutRows, int32& OutCols) const
{
	OutRows = Rows.Num();
	OutCols = (OutRows > 0) ? Rows[0].Values.Num() : 0;

	if (OutRows == 0 || OutCols == 0)
	{
		return false;
	}

	for (int32 r = 1; r < OutRows; ++r)
	{
		if (Rows[r].Values.Num() != OutCols)
		{
			return false; // non-rectangular
		}
	}

	OutMatrix.SetNum(OutRows);
	for (int32 r = 0; r < OutRows; ++r)
	{
		OutMatrix[r] = Rows[r].Values;
	}

	return true;
}

void UCrossCorrelation2DComponent::MatrixToRows(const TArray<TArray<float>>& Matrix, TArray<FFloatRow>& OutRows)
{
	OutRows.SetNum(Matrix.Num());
	for (int32 r = 0; r < Matrix.Num(); ++r)
	{
		OutRows[r].Values = Matrix[r];
	}
}
