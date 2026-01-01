#include "HyperDimensionalVectorComponent.h"

#include "Logging/LogMacros.h"

UHyperDimensionalVectorComponent::UHyperDimensionalVectorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHyperDimensionalVectorComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize(Dimensions);
}

void UHyperDimensionalVectorComponent::Initialize(int32 InDimensions)
{
	Dimensions = FMath::Max(1, InDimensions);
	const int32 NumVectors = FMath::CeilToInt(static_cast<float>(Dimensions) / 4.0f);
	Components.SetNumZeroed(NumVectors);
}

void UHyperDimensionalVectorComponent::SetDimension(int32 Index, float Value)
{
	if (Components.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("HyperDimensionalVectorComponent: Components array is not initialized. Call Initialize() first."));
		return;
	}

	const int32 VectorIndex = Index / 4;
	const int32 ComponentIndex = Index % 4;

	if (!Components.IsValidIndex(VectorIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Index out of range: Dimension %d is beyond the initialized size."), Index);
		return;
	}

	FVector4 Vec = Components[VectorIndex];
	switch (ComponentIndex)
	{
	case 0: Vec.X = Value; break;
	case 1: Vec.Y = Value; break;
	case 2: Vec.Z = Value; break;
	case 3: Vec.W = Value; break;
	default: break;
	}
	Components[VectorIndex] = Vec;
}

float UHyperDimensionalVectorComponent::GetDimension(int32 Index) const
{
	if (Components.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("HyperDimensionalVectorComponent: Components array is not initialized. Call Initialize() first."));
		return 0.f;
	}

	const int32 VectorIndex = Index / 4;
	const int32 ComponentIndex = Index % 4;

	if (!Components.IsValidIndex(VectorIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Index out of range: Dimension %d is beyond the initialized size."), Index);
		return 0.f;
	}

	const FVector4& Vec = Components[VectorIndex];
	switch (ComponentIndex)
	{
	case 0: return Vec.X;
	case 1: return Vec.Y;
	case 2: return Vec.Z;
	case 3: return Vec.W;
	default: return 0.f;
	}
}

void UHyperDimensionalVectorComponent::Normalize()
{
	if (Components.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("HyperDimensionalVectorComponent: Components array is not initialized. Call Initialize() first."));
		return;
	}

	double MagnitudeSq = 0.0;
	for (const FVector4& Vec : Components)
	{
		MagnitudeSq += static_cast<double>(Vec.X) * Vec.X +
			static_cast<double>(Vec.Y) * Vec.Y +
			static_cast<double>(Vec.Z) * Vec.Z +
			static_cast<double>(Vec.W) * Vec.W;
	}
	const double Magnitude = FMath::Sqrt(MagnitudeSq);
	if (Magnitude > KINDA_SMALL_NUMBER)
	{
		for (FVector4& Vec : Components)
		{
			Vec /= static_cast<float>(Magnitude);
		}
	}
}

float UHyperDimensionalVectorComponent::DotProduct(const UHyperDimensionalVectorComponent* Other) const
{
	if (!Other || Dimensions != Other->Dimensions)
	{
		UE_LOG(LogTemp, Error, TEXT("Dimension mismatch: Both vectors must have the same number of dimensions."));
		return 0.f;
	}

	const int32 NumVectors = Components.Num();
	float Dot = 0.f;
	for (int32 i = 0; i < NumVectors; ++i)
	{
		const FVector4& A = Components[i];
		const FVector4& B = Other->Components.IsValidIndex(i) ? Other->Components[i] : FVector4::Zero();
		Dot += A.X * B.X + A.Y * B.Y + A.Z * B.Z + A.W * B.W;
	}
	return Dot;
}
