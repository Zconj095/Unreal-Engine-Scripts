#include "BilinearInterpolationComponent.h"

#include "Logging/LogMacros.h"

UBilinearInterpolationComponent::UBilinearInterpolationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBilinearInterpolationComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!Texture)
    {
        UE_LOG(LogTemp, Error, TEXT("BilinearInterpolationComponent: Texture is not assigned."));
        return;
    }

    const FLinearColor Color = BilinearSample(Texture, SamplePoint);
    UE_LOG(
        LogTemp,
        Log,
        TEXT("Sampled color at (%0.2f, %0.2f): R=%0.3f G=%0.3f B=%0.3f A=%0.3f"),
        SamplePoint.X,
        SamplePoint.Y,
        Color.R,
        Color.G,
        Color.B,
        Color.A);
}

FLinearColor UBilinearInterpolationComponent::BilinearSample(UTexture2D* SourceTexture, FVector2D Point) const
{
    if (!SourceTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("BilinearInterpolationComponent: SourceTexture is null."));
        return FLinearColor::Black;
    }

    FTexturePlatformData* PlatformData = SourceTexture->GetPlatformData();
    if (!PlatformData || PlatformData->Mips.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("BilinearInterpolationComponent: Texture %s has no platform data."), *SourceTexture->GetName());
        return FLinearColor::Black;
    }

    const int32 Width = PlatformData->Mips[0].SizeX;
    const int32 Height = PlatformData->Mips[0].SizeY;
    if (Width <= 0 || Height <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("BilinearInterpolationComponent: Texture %s has invalid dimensions."), *SourceTexture->GetName());
        return FLinearColor::Black;
    }

    // Clamp normalized UVs
    Point.X = FMath::Clamp(Point.X, 0.f, 1.f);
    Point.Y = FMath::Clamp(Point.Y, 0.f, 1.f);

    const float ScaledX = Point.X * (Width - 1);
    const float ScaledY = Point.Y * (Height - 1);

    const int32 X0 = FMath::FloorToInt(ScaledX);
    const int32 Y0 = FMath::FloorToInt(ScaledY);
    const int32 X1 = FMath::Min(X0 + 1, Width - 1);
    const int32 Y1 = FMath::Min(Y0 + 1, Height - 1);

    const float U = ScaledX - X0;
    const float V = ScaledY - Y0;

    FTexture2DMipMap& Mip = PlatformData->Mips[0];
    const FColor* PixelData = static_cast<const FColor*>(Mip.BulkData.LockReadOnly());
    if (!PixelData)
    {
        UE_LOG(LogTemp, Warning, TEXT("BilinearInterpolationComponent: Failed to lock mip data for %s."), *SourceTexture->GetName());
        Mip.BulkData.Unlock();
        return FLinearColor::Black;
    }

    const int32 RowStride = Width;
    auto SamplePixel = [&](int32 X, int32 Y) -> FLinearColor
    {
        return FLinearColor(PixelData[Y * RowStride + X]);
    };

    const FLinearColor C00 = SamplePixel(X0, Y0);
    const FLinearColor C10 = SamplePixel(X1, Y0);
    const FLinearColor C01 = SamplePixel(X0, Y1);
    const FLinearColor C11 = SamplePixel(X1, Y1);

    const FLinearColor Interpolated =
        (1.f - U) * (1.f - V) * C00 +
        U * (1.f - V) * C10 +
        (1.f - U) * V * C01 +
        U * V * C11;

    Mip.BulkData.Unlock();
    return Interpolated;
}
