#include "ElectronSignal.h"
#include "Camera/CameraComponent.h"
#include "MaterialPropertiesComponent.h"
#include "DrawDebugHelpers.h"

AElectronSignal::AElectronSignal()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SignalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SignalCamera"));
	SignalCamera->SetupAttachment(Root);
	SignalCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	SignalCamera->OrthoWidth = 10.0f;
	SignalCamera->bUsePawnControlRotation = false;
}

void AElectronSignal::BeginPlay()
{
	Super::BeginPlay();

	InitializeCamera();
	InitializeTexture();
}

void AElectronSignal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!SignalTexture || Resolution <= 0)
	{
		return;
	}

	ComputeSignal();
}

void AElectronSignal::InitializeCamera()
{
	if (!SignalCamera)
	{
		return;
	}

	SignalCamera->SetWorldLocation(GetActorLocation() + GetActorForwardVector() * -10.0f);
	SignalCamera->SetWorldRotation(GetActorRotation());
	SignalCamera->UpdateComponentToWorld();
}

void AElectronSignal::InitializeTexture()
{
	if (Resolution <= 0)
	{
		return;
	}

	SignalPixels.SetNumZeroed(Resolution * Resolution);

	SignalTexture = UTexture2D::CreateTransient(Resolution, Resolution, PF_B8G8R8A8);
	if (SignalTexture)
	{
		SignalTexture->CompressionSettings = TC_VectorDisplacementmap;
		SignalTexture->SRGB = false;
		SignalTexture->AddToRoot();
		SignalTexture->UpdateResource();
	}
}

void AElectronSignal::ComputeSignal()
{
	const FVector CameraLocation = SignalCamera->GetComponentLocation();
	const FVector CameraForward = SignalCamera->GetForwardVector();
	const FVector CameraRight = SignalCamera->GetRightVector();
	const FVector CameraUp = SignalCamera->GetUpVector();
	const float HalfWidth = SignalCamera->OrthoWidth * 0.5f;
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
			const FVector RayEnd = RayOrigin + CameraForward * 10000.0f;

			FHitResult Hit;
			FColor SampleColor = FColor::Black;

			if (GetWorld()->LineTraceSingleByChannel(Hit, RayOrigin, RayEnd, static_cast<ECollisionChannel>(SampleChannel.GetValue()), QueryParams))
			{
				float AtomicNumber = 1.0f;
				if (const UMaterialPropertiesComponent* Props = Hit.GetActor()->FindComponentByClass<UMaterialPropertiesComponent>())
				{
					AtomicNumber = FMath::Max(1.0f, Props->AtomicNumber);
				}

				const float SignalValue = FMath::Clamp(AtomicNumber * BeamIntensity, 0.0f, 1.0f);
				const uint8 SignalByte = static_cast<uint8>(SignalValue * 255.0f);
				SampleColor = FColor(SignalByte, SignalByte, SignalByte, 255);
			}

			SignalPixels[X + Y * Resolution] = SampleColor;
		}
	}

	UpdateTexture(SignalTexture, SignalPixels);
}

void AElectronSignal::UpdateTexture(UTexture2D* Texture, const TArray<FColor>& Pixels)
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
