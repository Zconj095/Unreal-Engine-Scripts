#include "ElectronDetector.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "MaterialPropertiesComponent.h"

AElectronDetector::AElectronDetector()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	DetectionCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DetectionCamera"));
	DetectionCamera->SetupAttachment(Root);
	DetectionCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	DetectionCamera->OrthoWidth = 10.0f;
	DetectionCamera->bUsePawnControlRotation = false;
}

void AElectronDetector::BeginPlay()
{
	Super::BeginPlay();

	InitializeTextures();
}

void AElectronDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValidSetup())
	{
		return;
	}

	UpdateDetection();
}

void AElectronDetector::InitializeTextures()
{
	if (Resolution <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ElectronDetector: Resolution must be greater than zero."));
		return;
	}

	if (TopographicTexture == nullptr || TopographicTexture->GetSizeX() != Resolution)
	{
		TopographicTexture = UTexture2D::CreateTransient(Resolution, Resolution, PF_B8G8R8A8);
		TopographicTexture->CompressionSettings = TC_VectorDisplacementmap;
		TopographicTexture->SRGB = false;
		TopographicTexture->AddToRoot();
		TopographicTexture->UpdateResource();
	}

	if (CompositionalTexture == nullptr || CompositionalTexture->GetSizeX() != Resolution)
	{
		CompositionalTexture = UTexture2D::CreateTransient(Resolution, Resolution, PF_B8G8R8A8);
		CompositionalTexture->CompressionSettings = TC_VectorDisplacementmap;
		CompositionalTexture->SRGB = false;
		CompositionalTexture->AddToRoot();
		CompositionalTexture->UpdateResource();
	}

	const int32 TotalPixels = Resolution * Resolution;
	TopographicPixels.SetNumZeroed(TotalPixels);
	CompositionalPixels.SetNumZeroed(TotalPixels);
}

void AElectronDetector::UpdateDetection()
{
	if (TopographicPixels.Num() == 0 || CompositionalPixels.Num() == 0)
	{
		return;
	}

	const FVector CameraLocation = DetectionCamera->GetComponentLocation();
	const FVector CameraForward = DetectionCamera->GetForwardVector();
	const FVector CameraRight = DetectionCamera->GetRightVector();
	const FVector CameraUp = DetectionCamera->GetUpVector();
	const float HalfWidth = DetectionCamera->OrthoWidth * 0.5f;
	const float HalfHeight = HalfWidth;
	const float StepX = (HalfWidth * 2.0f) / Resolution;
	const float StepY = (HalfHeight * 2.0f) / Resolution;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	for (int32 Y = 0; Y < Resolution; ++Y)
	{
		for (int32 X = 0; X < Resolution; ++X)
		{
			const float OffsetX = (X + 0.5f) * StepX - HalfWidth;
			const float OffsetY = (Y + 0.5f) * StepY - HalfHeight;
			const FVector RayOrigin = CameraLocation + CameraRight * OffsetX + CameraUp * OffsetY;
			const FVector RayEnd = RayOrigin + CameraForward * TraceDistance;

			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, RayOrigin, RayEnd, SampleChannel, QueryParams))
			{
				const float TopographicIntensity = FMath::Clamp(FVector::DotProduct(Hit.Normal, -CameraForward) * TopographicMultiplier, 0.0f, 1.0f);
				float AtomicNumber = 1.0f;
				if (const UMaterialPropertiesComponent* Props = Hit.GetActor()->FindComponentByClass<UMaterialPropertiesComponent>())
				{
					AtomicNumber = FMath::Max(1.0f, Props->AtomicNumber);
				}

				const float CompositionalIntensity = FMath::Clamp(AtomicNumber * CompositionalMultiplier, 0.0f, 1.0f);
				const int32 PixelIndex = X + Y * Resolution;
				TopographicPixels[PixelIndex] = MakeMonochromeColor(TopographicIntensity);
				CompositionalPixels[PixelIndex] = MakeMonochromeColor(CompositionalIntensity);
			}
			else
			{
				const int32 PixelIndex = X + Y * Resolution;
				TopographicPixels[PixelIndex] = FColor::Black;
				CompositionalPixels[PixelIndex] = FColor::Black;
			}
		}
	}

	UpdateTextureData(TopographicTexture, TopographicPixels);
	UpdateTextureData(CompositionalTexture, CompositionalPixels);
}

void AElectronDetector::UpdateTextureData(UTexture2D* Texture, const TArray<FColor>& Pixels)
{
	FTexturePlatformData* PlatformData = Texture ? Texture->GetPlatformData() : nullptr;
	if (!PlatformData || Pixels.Num() == 0)
	{
		return;
	}

	void* TextureData = PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
	PlatformData->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();
}

FColor AElectronDetector::MakeMonochromeColor(float Intensity) const
{
	const uint8 Value = static_cast<uint8>(FMath::Clamp(Intensity, 0.0f, 1.0f) * 255.0f);
	return FColor(Value, Value, Value, 255);
}

bool AElectronDetector::IsValidSetup() const
{
	return Resolution > 0 && DetectionCamera != nullptr && TopographicTexture != nullptr && CompositionalTexture != nullptr;
}
