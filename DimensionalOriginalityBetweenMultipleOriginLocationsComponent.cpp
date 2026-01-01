#include "DimensionalOriginalityBetweenMultipleOriginLocationsComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Logging/LogMacros.h"

UDimensionalOriginalityBetweenMultipleOriginLocationsComponent::UDimensionalOriginalityBetweenMultipleOriginLocationsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDimensionalOriginalityBetweenMultipleOriginLocationsComponent::BeginPlay()
{
	Super::BeginPlay();

	CompareOrigins();
}

void UDimensionalOriginalityBetweenMultipleOriginLocationsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Example: recalc when 'U' is pressed; also nudge first origin along +X (cm units).
	if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		if (PC->WasInputKeyJustPressed(EKeys::U))
		{
			if (OriginPoints.Num() > 0 && OriginPoints[0])
			{
				const FVector Pos = OriginPoints[0]->GetActorLocation();
				OriginPoints[0]->SetActorLocation(Pos + FVector(10.f, 0.f, 0.f)); // 10 cm step
			}
			CompareOrigins();
		}
	}

	if (bDrawDebug)
	{
		DrawDebug();
	}
}

void UDimensionalOriginalityBetweenMultipleOriginLocationsComponent::ResizeMatrices(int32 Count)
{
	Distances.SetNum(Count);
	Angles.SetNum(Count);
	for (int32 i = 0; i < Count; ++i)
	{
		Distances[i].Values.Init(0.f, Count);
		Angles[i].Values.Init(0.f, Count);
	}
}

void UDimensionalOriginalityBetweenMultipleOriginLocationsComponent::CompareOrigins()
{
	const int32 OriginCount = OriginPoints.Num();
	if (OriginCount == 0)
	{
		return;
	}

	const int32 EffectiveCount = FMath::Clamp(ComparisonCount, 0, OriginCount);
	ResizeMatrices(OriginCount);

	const FVector Reference = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;

	for (int32 i = 0; i < EffectiveCount; ++i)
	{
		AActor* OriginI = OriginPoints[i];
		if (!OriginI) { continue; }

		const FVector PosI = OriginI->GetActorLocation();
		const FVector DirI = PosI - Reference;

		for (int32 j = i + 1; j < EffectiveCount; ++j)
		{
			AActor* OriginJ = OriginPoints[j];
			if (!OriginJ) { continue; }

			const FVector PosJ = OriginJ->GetActorLocation();
			const FVector DirJ = PosJ - Reference;

			Distances[i].Values[j] = FVector::Dist(PosI, PosJ);

			float AngleDeg = 0.f;
			const double Dot = DirI | DirJ;
			const double MagI = DirI.Size();
			const double MagJ = DirJ.Size();
			if (MagI > KINDA_SMALL_NUMBER && MagJ > KINDA_SMALL_NUMBER)
			{
				const double CosTheta = FMath::Clamp(Dot / (MagI * MagJ), -1.0, 1.0);
				AngleDeg = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
			}
			Angles[i].Values[j] = AngleDeg;

			UE_LOG(LogTemp, Log, TEXT("Distance between Origin %d and Origin %d: %f"), i, j, Distances[i].Values[j]);
			UE_LOG(LogTemp, Log, TEXT("Angle between Origin %d and Origin %d: %f"), i, j, Angles[i].Values[j]);
		}
	}
}

void UDimensionalOriginalityBetweenMultipleOriginLocationsComponent::UpdateOrigin(int32 Index, AActor* NewOrigin)
{
	if (Index >= 0 && Index < OriginPoints.Num())
	{
		OriginPoints[Index] = NewOrigin;
		CompareOrigins();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid origin index provided: %d"), Index);
	}
}

void UDimensionalOriginalityBetweenMultipleOriginLocationsComponent::ApplyDimensionalTransformation()
{
	if (!CustomTransform)
	{
		UE_LOG(LogTemp, Warning, TEXT("Custom transform is not set."));
		return;
	}

	const FTransform Xform = CustomTransform->GetComponentTransform();

	for (AActor* Origin : OriginPoints)
	{
		if (Origin)
		{
			const FVector NewPos = Xform.TransformPosition(Origin->GetActorLocation());
			Origin->SetActorLocation(NewPos);
		}
	}

	CompareOrigins();
}

void UDimensionalOriginalityBetweenMultipleOriginLocationsComponent::DrawDebug() const
{
	if (!GetWorld() || OriginPoints.Num() == 0)
	{
		return;
	}

	for (AActor* Origin : OriginPoints)
	{
		if (Origin)
		{
			DrawDebugSphere(GetWorld(), Origin->GetActorLocation(), DebugSphereRadius, 12, FColor::Red, false, -1.f, 0, 1.f);
		}
	}

	for (int32 i = 0; i < OriginPoints.Num(); ++i)
	{
		AActor* A = OriginPoints[i];
		if (!A) { continue; }
		for (int32 j = i + 1; j < OriginPoints.Num(); ++j)
		{
			AActor* B = OriginPoints[j];
			if (!B) { continue; }
			DrawDebugLine(GetWorld(), A->GetActorLocation(), B->GetActorLocation(), FColor::Green, false, -1.f, 0, 1.f);
		}
	}
}
