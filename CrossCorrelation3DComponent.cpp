#include "CrossCorrelation3DComponent.h"

#include "Logging/LogMacros.h"

UCrossCorrelation3DComponent::UCrossCorrelation3DComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Default volume matching the Unity example
	Volume = {
		FFloatPlane3D{ { FFloatRow3D{{1.f, 2.f}}, FFloatRow3D{{3.f, 4.f}} } },
		FFloatPlane3D{ { FFloatRow3D{{5.f, 6.f}}, FFloatRow3D{{7.f, 8.f}} } }
	};

	// Default kernel matching the Unity example
	Kernel = {
		FFloatPlane3D{ { FFloatRow3D{{1.f, 0.f}}, FFloatRow3D{{0.f, -1.f}} } },
		FFloatPlane3D{ { FFloatRow3D{{1.f, 0.f}}, FFloatRow3D{{0.f, -1.f}} } }
	};
}

void UCrossCorrelation3DComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FFloatPlane3D> Result;
	ComputeCrossCorrelation3D(Volume, Kernel, Result);

	if (Result.Num() == 0)
	{
		// Errors already logged in ComputeCrossCorrelation3D
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("3D Cross Correlation Result:"));
	for (int32 i = 0; i < Result.Num(); ++i)
	{
		const FFloatPlane3D& Plane = Result[i];
		for (int32 j = 0; j < Plane.Rows.Num(); ++j)
		{
			const FFloatRow3D& Row = Plane.Rows[j];
			FString RowStr;
			for (int32 k = 0; k < Row.Values.Num(); ++k)
			{
				RowStr += FString::Printf(TEXT("%.2f"), Row.Values[k]);
				if (k + 1 < Row.Values.Num())
				{
					RowStr += TEXT(" ");
				}
			}
			UE_LOG(LogTemp, Log, TEXT("%s"), *RowStr);
		}
	}
}

void UCrossCorrelation3DComponent::ComputeCrossCorrelation3D(const TArray<FFloatPlane3D>& InVolume, const TArray<FFloatPlane3D>& InKernel, TArray<FFloatPlane3D>& OutResult) const
{
	OutResult.Reset();

	TArray<TArray<TArray<float>>> VolMat;
	TArray<TArray<TArray<float>>> KerMat;
	int32 VolX = 0, VolY = 0, VolZ = 0;
	int32 KerX = 0, KerY = 0, KerZ = 0;

	if (!ToMatrix(InVolume, VolMat, VolX, VolY, VolZ) || !ToMatrix(InKernel, KerMat, KerX, KerY, KerZ))
	{
		UE_LOG(LogTemp, Error, TEXT("CrossCorrelation3DComponent: Invalid input - ensure volume and kernel are non-empty and rectangular."));
		return;
	}

	if (KerX > VolX || KerY > VolY || KerZ > VolZ)
	{
		UE_LOG(LogTemp, Error, TEXT("CrossCorrelation3DComponent: Kernel dimensions are larger than the volume dimensions. Cross-correlation not possible."));
		return;
	}

	const int32 OutX = VolX - KerX + 1;
	const int32 OutY = VolY - KerY + 1;
	const int32 OutZ = VolZ - KerZ + 1;

	TArray<TArray<TArray<float>>> Corr;
	Corr.SetNum(OutX);
	for (int32 x = 0; x < OutX; ++x)
	{
		Corr[x].SetNum(OutY);
		for (int32 y = 0; y < OutY; ++y)
		{
			Corr[x][y].SetNumZeroed(OutZ);
		}
	}

	for (int32 i = 0; i < OutX; ++i)
	{
		for (int32 j = 0; j < OutY; ++j)
		{
			for (int32 k = 0; k < OutZ; ++k)
			{
				float Sum = 0.f;
				for (int32 m = 0; m < KerX; ++m)
				{
					for (int32 n = 0; n < KerY; ++n)
					{
						for (int32 p = 0; p < KerZ; ++p)
						{
							Sum += VolMat[i + m][j + n][k + p] * KerMat[m][n][p];
						}
					}
				}
				Corr[i][j][k] = Sum;
			}
		}
	}

	MatrixToPlanes(Corr, OutResult);
}

bool UCrossCorrelation3DComponent::ToMatrix(const TArray<FFloatPlane3D>& In, TArray<TArray<TArray<float>>>& OutMatrix, int32& OutX, int32& OutY, int32& OutZ) const
{
	OutX = In.Num();
	OutY = (OutX > 0 && In[0].Rows.Num() > 0) ? In[0].Rows.Num() : 0;
	OutZ = (OutY > 0) ? In[0].Rows[0].Values.Num() : 0;

	if (OutX == 0 || OutY == 0 || OutZ == 0)
	{
		return false;
	}

	// Validate rectangular structure
	for (int32 x = 0; x < OutX; ++x)
	{
		if (In[x].Rows.Num() != OutY)
		{
			return false;
		}
		for (int32 y = 0; y < OutY; ++y)
		{
			if (In[x].Rows[y].Values.Num() != OutZ)
			{
				return false;
			}
		}
	}

	OutMatrix.SetNum(OutX);
	for (int32 x = 0; x < OutX; ++x)
	{
		OutMatrix[x].SetNum(OutY);
		for (int32 y = 0; y < OutY; ++y)
		{
			OutMatrix[x][y] = In[x].Rows[y].Values;
		}
	}

	return true;
}

void UCrossCorrelation3DComponent::MatrixToPlanes(const TArray<TArray<TArray<float>>>& Matrix, TArray<FFloatPlane3D>& OutPlanes)
{
	OutPlanes.SetNum(Matrix.Num());
	for (int32 x = 0; x < Matrix.Num(); ++x)
	{
		const TArray<TArray<float>>& Plane = Matrix[x];
		OutPlanes[x].Rows.SetNum(Plane.Num());
		for (int32 y = 0; y < Plane.Num(); ++y)
		{
			OutPlanes[x].Rows[y].Values = Plane[y];
		}
	}
}
