#include "ImageProcessor.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"

namespace
{
    static bool DecodeToRGBA8(const TArray<uint8>& Compressed, TArray<uint8>& OutRGBA, int32& OutW, int32& OutH)
    {
        if (Compressed.Num() == 0)
        {
            return false;
        }

        IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

        TArray<EImageFormat> CandidateFormats;
        // Try detected format first if available
        {
            EImageFormat Detected = ImageWrapperModule.DetectImageFormat(Compressed.GetData(), Compressed.Num());
            if (Detected != EImageFormat::Invalid)
            {
                CandidateFormats.Add(Detected);
            }
        }
        // Fallback common formats
        CandidateFormats.Append({EImageFormat::PNG, EImageFormat::JPEG, EImageFormat::BMP, EImageFormat::ICO, EImageFormat::EXR});

        for (EImageFormat Format : CandidateFormats)
        {
            TSharedPtr<IImageWrapper> Wrapper = ImageWrapperModule.CreateImageWrapper(Format);
            if (!Wrapper.IsValid())
            {
                continue;
            }
            if (!Wrapper->SetCompressed(Compressed.GetData(), Compressed.Num()))
            {
                continue;
            }

            OutW = Wrapper->GetWidth();
            OutH = Wrapper->GetHeight();
            if (OutW <= 0 || OutH <= 0)
            {
                continue;
            }

            if (Wrapper->GetRaw(ERGBFormat::RGBA, 8, OutRGBA))
            {
                return true;
            }
        }

        return false;
    }

    static void ResizeBilinearGrayscale(const TArray<float>& Src, int32 SW, int32 SH, int32 DW, int32 DH, TArray<float>& Dst)
    {
        Dst.SetNumUninitialized(DW * DH);
        if (SW <= 0 || SH <= 0 || DW <= 0 || DH <= 0)
        {
            return;
        }

        const float ScaleX = static_cast<float>(SW) / static_cast<float>(DW);
        const float ScaleY = static_cast<float>(SH) / static_cast<float>(DH);

        for (int32 y = 0; y < DH; ++y)
        {
            const float SrcY = (static_cast<float>(y) + 0.5f) * ScaleY - 0.5f;
            int32 y0 = FMath::FloorToInt(SrcY);
            int32 y1 = FMath::Min(y0 + 1, SH - 1);
            const float wy = SrcY - static_cast<float>(y0);
            y0 = FMath::Clamp(y0, 0, SH - 1);

            for (int32 x = 0; x < DW; ++x)
            {
                const float SrcX = (static_cast<float>(x) + 0.5f) * ScaleX - 0.5f;
                int32 x0 = FMath::FloorToInt(SrcX);
                int32 x1 = FMath::Min(x0 + 1, SW - 1);
                const float wx = SrcX - static_cast<float>(x0);
                x0 = FMath::Clamp(x0, 0, SW - 1);

                const float p00 = Src[y0 * SW + x0];
                const float p10 = Src[y0 * SW + x1];
                const float p01 = Src[y1 * SW + x0];
                const float p11 = Src[y1 * SW + x1];

                const float p0 = FMath::Lerp(p00, p10, wx);
                const float p1 = FMath::Lerp(p01, p11, wx);
                const float p = FMath::Lerp(p0, p1, wy);

                Dst[y * DW + x] = p;
            }
        }
    }
}

bool FImageProcessor::ConvertImageToEmbedding(const TArray<uint8>& ImageData,
                                              TArray<float>& OutEmbedding,
                                              int32 TargetWidth,
                                              int32 TargetHeight)
{
    OutEmbedding.Reset();

    TArray<uint8> RGBA;
    int32 SrcW = 0, SrcH = 0;
    if (!DecodeToRGBA8(ImageData, RGBA, SrcW, SrcH))
    {
        return false;
    }

    // Convert to grayscale [0,1]
    const int32 NumPixels = SrcW * SrcH;
    TArray<float> Gray;
    Gray.SetNumUninitialized(NumPixels);
    for (int32 i = 0; i < NumPixels; ++i)
    {
        const uint8 r = RGBA[4 * i + 0];
        const uint8 g = RGBA[4 * i + 1];
        const uint8 b = RGBA[4 * i + 2];
        Gray[i] = (static_cast<float>(r) + static_cast<float>(g) + static_cast<float>(b)) / (3.0f * 255.0f);
    }

    // Resize to target resolution
    ResizeBilinearGrayscale(Gray, SrcW, SrcH, TargetWidth, TargetHeight, OutEmbedding);

    return OutEmbedding.Num() == TargetWidth * TargetHeight;
}

