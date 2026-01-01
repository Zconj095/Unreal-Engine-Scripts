#include "EnigmaResponseResonanceComponent.h"

#include "Components/AudioComponent.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

UEnigmaResponseResonanceComponent::UEnigmaResponseResonanceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnigmaResponseResonanceComponent::BeginPlay()
{
	Super::BeginPlay();

	InitVisualTargets();

	if (ResonanceAudioComponent)
	{
		ResonanceAudioComponent->Stop();
	}
}

void UEnigmaResponseResonanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float InteractionStrength = GetPuzzleInteractionStrength();

	if (InteractionStrength > PuzzleInteractionThreshold)
	{
		StartResonance();
	}

	if (bIsResonating)
	{
		UpdateResonanceEffects();

		ResonanceTimer += DeltaTime;
		if (ResonanceTimer >= ResonanceDuration)
		{
			StopResonance();
		}
	}
	else
	{
		ResetResonanceEffects();
	}
}

float UEnigmaResponseResonanceComponent::GetPuzzleInteractionStrength() const
{
	if (!GetWorld())
	{
		return 0.0f;
	}

	// Simulated ping-pong interaction strength
	const float Time = GetWorld()->GetTimeSeconds();
	return FMath::Abs(FMath::Fmod(Time * 0.5f, 2.0f) - 1.0f);
}

void UEnigmaResponseResonanceComponent::StartResonance()
{
	if (!bIsResonating)
	{
		bIsResonating = true;
		ResonanceTimer = 0.0f;
		if (ResonanceAudioComponent)
		{
			ResonanceAudioComponent->Play();
		}
	}
}

void UEnigmaResponseResonanceComponent::StopResonance()
{
	bIsResonating = false;
	ResonanceTimer = 0.0f;

	if (ResonanceAudioComponent)
	{
		ResonanceAudioComponent->Stop();
	}
}

void UEnigmaResponseResonanceComponent::UpdateResonanceEffects()
{
	const float Time = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	CurrentResonanceStrength = FMath::Abs(FMath::Fmod(Time * ResonanceFrequency, 2.0f) - 1.0f);
	ApplyResonanceEffects(CurrentResonanceStrength);
}

void UEnigmaResponseResonanceComponent::ApplyResonanceEffects(float Strength)
{
	if (ResonanceMID)
	{
		ResonanceMID->SetScalarParameterValue(TEXT("Glossiness"), Strength);
		const FLinearColor Color = FLinearColor::LerpUsingHSV(FLinearColor::White, FLinearColor::Blue, Strength);
		ResonanceMID->SetVectorParameterValue(TEXT("Color"), Color);
	}
}

void UEnigmaResponseResonanceComponent::ResetResonanceEffects()
{
	CurrentResonanceStrength = 0.0f;
	if (ResonanceMID)
	{
		ResonanceMID->SetScalarParameterValue(TEXT("Glossiness"), 0.0f);
		ResonanceMID->SetVectorParameterValue(TEXT("Color"), FLinearColor::White);
	}
}

void UEnigmaResponseResonanceComponent::InitVisualTargets()
{
	if (!TargetMesh && GetOwner())
	{
		TArray<UMeshComponent*> Meshes;
		GetOwner()->GetComponents<UMeshComponent>(Meshes);
		if (Meshes.Num() > 0)
		{
			TargetMesh = Meshes[0];
		}
	}

	UMaterialInterface* BaseMat = ResonanceMaterial;
	if (!BaseMat && TargetMesh && TargetMesh->GetNumMaterials() > 0)
	{
		BaseMat = TargetMesh->GetMaterial(0);
	}

	if (BaseMat && TargetMesh)
	{
		ResonanceMID = TargetMesh->CreateDynamicMaterialInstance(0, BaseMat);
	}
}
