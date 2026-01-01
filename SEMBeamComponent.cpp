#include "SEMBeamComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "NanoParticleComponent.h"
#include "Engine/Texture2D.h"

USEMBeamComponent::USEMBeamComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USEMBeamComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeCamera();
	InitializeTexture();
}

void USEMBeamComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!SEMCamera || !ScanTexture)
	{
		return;
	}

	PerformScan();

	if (bTextureDirty)
	{
		UpdateTextureRegion();
		bTextureDirty = false;
	}
}

void USEMBeamComponent::InitializeCamera()
{
	if (SEMCamera)
	{
		return;
	}

	if (AActor* Owner = GetOwner())
	{
		SEMCamera = Owner->FindComponentByClass<UCameraComponent>();
	}

	if (!SEMCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("SEMBeamComponent requires a CameraComponent on the owner."));
	}
}

void USEMBeamComponent::InitializeTexture()
{
	if (Resolution <= 0)
	{
		return;
	}

	PixelBuffer.SetNumZeroed(Resolution * Resolution);

	if (!ScanTexture || ScanTexture->GetSizeX() != Resolution)
	{
		ScanTexture = UTexture2D::CreateTransient(Resolution, Resolution, PF_B8G8R8A8);
		ScanTexture->CompressionSettings = TC_VectorDisplacementmap;
		ScanTexture->SRGB = false;
		ScanTexture->AddToRoot();
		ScanTexture->UpdateResource();
	}
}

void USEMBeamComponent::PerformScan()
{
	if (Resolution <= 0)
	{
		return;
	}

	for (int32 Sample = 0; Sample < SamplesPerTick; ++Sample)
	{
		const float U = (CurrentX + 0.5f) / Resolution;
		const float V = (CurrentY + 0.5f) / Resolution;

		const FVector CameraLocation = SEMCamera->GetComponentLocation();
		const FVector Start = CameraLocation
			+ SEMCamera->GetRightVector() * (U - 0.5f) * SEMCamera->OrthoWidth
			+ SEMCamera->GetUpVector() * (V - 0.5f) * SEMCamera->OrthoWidth;
		const FVector End = Start + SEMCamera->GetForwardVector() * 10000.0f;

		FHitResult Hit;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TargetChannel);

		const float Intensity = bHit ? CalculateBackscatter(Hit) : 0.0f;
		const uint8 Grey = static_cast<uint8>(FMath::Clamp(Intensity, 0.0f, 1.0f) * 255.0f);
		const FColor PixelColor = FColor(Grey, Grey, Grey, 255);

		PixelBuffer[CurrentX + CurrentY * Resolution] = PixelColor;
		bTextureDirty = true;

		CurrentX++;
		if (CurrentX >= Resolution)
		{
			CurrentX = 0;
			CurrentY++;
			if (CurrentY >= Resolution)
			{
				CurrentY = 0;
			}
		}
	}
}

float USEMBeamComponent::CalculateBackscatter(const FHitResult& Hit) const
{
	if (!Hit.GetActor())
	{
		return 0.0f;
	}

	float Density = 1.0f;
	if (UNanoParticleComponent* Particle = Hit.GetActor()->FindComponentByClass<UNanoParticleComponent>())
	{
		if (AActor* ParticleOwner = Particle->GetOwner())
		{
			Density = ParticleOwner->GetActorScale3D().Size();
		}
	}

	const float Angle = FVector::DotProduct(Hit.Normal, FVector::UpVector);
	return FMath::Clamp(Density * FMath::Abs(Angle) * BeamIntensity, 0.0f, 1.0f);
}

void USEMBeamComponent::UpdateTextureRegion()
{
	if (!ScanTexture)
	{
		return;
	}

	FTexturePlatformData* PlatformData = ScanTexture->GetPlatformData();
	if (!PlatformData || PlatformData->Mips.Num() == 0)
	{
		return;
	}

	FTexture2DMipMap& Mip = PlatformData->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, PixelBuffer.GetData(), PixelBuffer.Num() * sizeof(FColor));
	Mip.BulkData.Unlock();
	ScanTexture->UpdateResource();
}
