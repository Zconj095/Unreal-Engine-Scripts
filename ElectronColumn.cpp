#include "ElectronColumn.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

AElectronColumn::AElectronColumn()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void AElectronColumn::BeginPlay()
{
	Super::BeginPlay();

	if (!ValidateSetup())
	{
		SetActorTickEnabled(false);
		return;
	}

	bHasValidated = true;
}

void AElectronColumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasValidated && !ValidateSetup())
	{
		return;
	}

	SimulateElectronBeam();

	if (bShowGizmos)
	{
		DrawDebugGizmos();
	}
}

#if WITH_EDITOR
void AElectronColumn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	bHasValidated = ValidateSetup();

	if (bShowGizmos && bHasValidated)
	{
		DrawDebugGizmos();
	}
}
#endif

bool AElectronColumn::ValidateSetup()
{
	if (ElectronSource == nullptr || Aperture == nullptr || Target == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ElectronColumn requires ElectronSource, Aperture, and Target to be assigned."));
		return false;
	}

	return true;
}

void AElectronColumn::SimulateElectronBeam()
{
	const FVector SourcePos = ElectronSource->GetComponentLocation();
	const FVector AperturePos = Aperture->GetComponentLocation();
	const FVector TargetPos = Target->GetComponentLocation();

	if (!GetWorld())
	{
		return;
	}

	const float BeamThickness = FMath::Max(0.01f, SourceBeamWidth);
	DrawDebugLine(GetWorld(), SourcePos, AperturePos, FColor::Cyan, false, 0.0f, 0, BeamThickness);
	DrawDebugLine(GetWorld(), AperturePos, TargetPos, FColor::Cyan, false, 0.0f, 0, ApertureBeamWidth);

	const float BeamRadius = FVector::Dist(SourcePos, AperturePos) * ApertureBeamWidth;
	if (BeamRadius > ApertureRadius && !bHasWarnedBeamTooWide)
	{
		UE_LOG(LogTemp, Warning, TEXT("Electron beam exceeds aperture radius by %.4f units."), BeamRadius - ApertureRadius);
		bHasWarnedBeamTooWide = true;
	}
}

void AElectronColumn::DrawDebugGizmos() const
{
	if (!GetWorld())
	{
		return;
	}

	if (ElectronSource)
	{
		DrawDebugSphere(GetWorld(), ElectronSource->GetComponentLocation(), SourceBeamWidth, 16, FColor::Green, false, 0.0f, 0, 1.0f);
	}

	if (Aperture)
	{
		DrawDebugSphere(GetWorld(), Aperture->GetComponentLocation(), ApertureRadius, 16, FColor::Green, false, 0.0f, 0, 1.0f);
	}

	if (Target)
	{
		DrawDebugSphere(GetWorld(), Target->GetComponentLocation(), 0.1f, 16, FColor::Red, false, 0.0f, 0, 1.0f);
	}
}
