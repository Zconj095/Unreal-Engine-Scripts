#include "NeuralMappingVisualizationComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/Package.h"

UNeuralMappingVisualizationComponent::UNeuralMappingVisualizationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralMappingVisualizationComponent::BeginPlay()
{
	Super::BeginPlay();

	GenerateNeuralMapGrid();
	RunTensorFlowPlaceholder();
}

void UNeuralMappingVisualizationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bVisualizeMapInWorld && bMapGenerated)
	{
		DrawNeuralMapDebug();
	}
}

void UNeuralMappingVisualizationComponent::GenerateNeuralMapGrid()
{
	Resolution = FMath::Clamp(Resolution, 8, 64);
	const int32 TotalPoints = Resolution * Resolution * Resolution;
	if (TotalPoints == 0)
	{
		bMapGenerated = false;
		return;
	}

	NeuralMap.SetNumZeroed(TotalPoints);
	const double Step = 1.0 / Resolution;

	for (int32 Z = 0; Z < Resolution; ++Z)
	{
		for (int32 Y = 0; Y < Resolution; ++Y)
		{
			for (int32 X = 0; X < Resolution; ++X)
			{
				const int32 Index = X + Resolution * (Y + Resolution * Z);
				const FVector Position(
					static_cast<float>(X * Step),
					static_cast<float>(Y * Step),
					static_cast<float>(Z * Step)
				);

				const FVector Gradient = ComputeGradient(Position, Step);
				NeuralMap[Index] = Gradient.Size() + MagicalEnhancementFactor;
			}
		}
	}

	bMapGenerated = true;
}

double UNeuralMappingVisualizationComponent::EvaluateNeuralDensity(const FVector& Position) const
{
	return FMath::Exp(-Position.Size());
}

FVector UNeuralMappingVisualizationComponent::ComputeGradient(const FVector& Position, double Step) const
{
	const FVector OffsetX((float)Step, 0.0f, 0.0f);
	const FVector OffsetY(0.0f, (float)Step, 0.0f);
	const FVector OffsetZ(0.0f, 0.0f, (float)Step);

	const double dNx = (EvaluateNeuralDensity(Position + OffsetX) - EvaluateNeuralDensity(Position - OffsetX)) / (2.0 * Step);
	const double dNy = (EvaluateNeuralDensity(Position + OffsetY) - EvaluateNeuralDensity(Position - OffsetY)) / (2.0 * Step);
	const double dNz = (EvaluateNeuralDensity(Position + OffsetZ) - EvaluateNeuralDensity(Position - OffsetZ)) / (2.0 * Step);

	return FVector(static_cast<float>(dNx), static_cast<float>(dNy), static_cast<float>(dNz));
}

void UNeuralMappingVisualizationComponent::LogNeuralMapStats() const
{
	if (!bMapGenerated || NeuralMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Neural map has not been generated yet."));
		return;
	}

	double MinValue = TNumericLimits<double>::Max();
	double MaxValue = TNumericLimits<double>::Lowest();
	double Sum = 0.0;

	for (double Value : NeuralMap)
	{
		MinValue = FMath::Min(MinValue, Value);
		MaxValue = FMath::Max(MaxValue, Value);
		Sum += Value;
	}

	const double Average = Sum / NeuralMap.Num();
	UE_LOG(LogTemp, Log, TEXT("Neural map stats -> Resolution: %d, Min: %.4f, Max: %.4f, Avg: %.4f"),
		Resolution, MinValue, MaxValue, Average);
}

void UNeuralMappingVisualizationComponent::RunTensorFlowPlaceholder() const
{
	const double SampleValue = EvaluateNeuralDensity(FVector::ZeroVector) + MagicalEnhancementFactor;
	UE_LOG(LogTemp, Log, TEXT("TensorFlow placeholder value: %.4f"), SampleValue);
}

void UNeuralMappingVisualizationComponent::DrawNeuralMapDebug() const
{
	if (!GetWorld() || NeuralMap.Num() == 0)
	{
		return;
	}

	const int32 Step = FMath::Max(1, Resolution / 8);
	const FVector OwnerLocation = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
	const float ScaleFactor = 100.0f;

	for (int32 Z = 0; Z < Resolution; Z += Step)
	{
		for (int32 Y = 0; Y < Resolution; Y += Step)
		{
			for (int32 X = 0; X < Resolution; X += Step)
			{
				const int32 Index = X + Resolution * (Y + Resolution * Z);
				const float Normalized = FMath::Clamp(static_cast<float>(NeuralMap[Index]) / (MagicalEnhancementFactor + 2.0f), 0.0f, 1.0f);
				const FVector LocalPosition(
					static_cast<float>(X) / Resolution,
					static_cast<float>(Y) / Resolution,
					static_cast<float>(Z) / Resolution
				);
				const FVector WorldPosition = OwnerLocation + LocalPosition * ScaleFactor;
				const FColor Color = FLinearColor(Normalized, 0.0f, 1.0f - Normalized).ToFColor(true);
				DrawDebugSphere(GetWorld(), WorldPosition, 1.0f, 4, Color, false, 0.0f, 0, 0.5f);
			}
		}
	}
}
