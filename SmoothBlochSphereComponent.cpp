// Copyright Epic Games, Inc.

#include "SmoothBlochSphereComponent.h"

#include "Components/MeshComponent.h"
#include "ComplexNumber.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

USmoothBlochSphereComponent::USmoothBlochSphereComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USmoothBlochSphereComponent::InitializeQubits(int32 NumQubits)
{
	for (TPair<int32, TWeakObjectPtr<AActor>>& Entry : QubitSpheres)
	{
		if (Entry.Value.IsValid())
		{
			Entry.Value->Destroy();
		}
	}
	QubitSpheres.Empty();

	if (!GetWorld() || NumQubits <= 0)
	{
		return;
	}

	for (int32 i = 0; i < NumQubits; ++i)
	{
		const FVector Position(i * QubitSpacing, 0.f, 0.f);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* Sphere = GetWorld()->SpawnActor<AActor>(SphereActorClass ? *SphereActorClass : AActor::StaticClass(), Position, FRotator::ZeroRotator, Params);
		if (Sphere)
		{
#if WITH_EDITOR
			Sphere->SetActorLabel(FString::Printf(TEXT("Qubit_%d_Bloch"), i));
#endif
			QubitSpheres.Add(i, Sphere);
		}
	}
}

void USmoothBlochSphereComponent::UpdateState(const TArray<FComplexNumber>& StateVector)
{
	const int32 NumStates = StateVector.Num();
	if (NumStates == 0)
	{
		return;
	}

	for (int32 i = 0; i < NumStates; ++i)
	{
		if (!QubitSpheres.Contains(i) || !QubitSpheres[i].IsValid())
		{
			continue;
		}

		AActor* Sphere = QubitSpheres[i].Get();
		if (UMeshComponent* Mesh = Sphere->FindComponentByClass<UMeshComponent>())
		{
			const FComplexNumber& Amp = StateVector[i];
			const float TargetR = FMath::Abs(static_cast<float>(Amp.Real));
			const float TargetG = FMath::Abs(static_cast<float>(Amp.Imaginary));
			const float TargetB = 1.0f - FMath::Abs(static_cast<float>(FMath::Sqrt(Amp.Real * Amp.Real + Amp.Imaginary * Amp.Imaginary)));

			const FLinearColor TargetColor(TargetR, TargetG, TargetB, 1.f);
			Mesh->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), FVector(TargetColor.R, TargetColor.G, TargetColor.B));
		}
	}
}
