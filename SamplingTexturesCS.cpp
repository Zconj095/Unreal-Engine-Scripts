// Implementation of SamplingTextures.usf dispatch helper

#include "SamplingTexturesCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FSamplingTexturesCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FSamplingTexturesCS);
    SHADER_USE_PARAMETER_STRUCT(FSamplingTexturesCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(int32, NumChunks)
        SHADER_PARAMETER_SRV(StructuredBuffer<int>, TextureIndices)
        SHADER_PARAMETER_TEXTURE(Texture2DArray<float4>, TextureArray)
        SHADER_PARAMETER_SAMPLER(SamplerState, LayerSampler)
        SHADER_PARAMETER_UAV(RWTexture2D<float4>, OutputTexture)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FSamplingTexturesCS, "/PEKBGGAP/SamplingTextures.usf", "CSMain", SF_Compute);

void DispatchSamplingTextures(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* TextureIndicesSRV,
    FRHITexture* TextureArrayRHI,
    FRHIUnorderedAccessView* OutputUAV,
    int32 NumChunks)
{
    check(TextureIndicesSRV != nullptr);
    check(TextureArrayRHI != nullptr);
    check(OutputUAV != nullptr);

    TShaderMapRef<FSamplingTexturesCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FSamplingTexturesCS::FParameters Params;
    Params.NumChunks = NumChunks;
    Params.TextureIndices = TextureIndicesSRV;
    Params.TextureArray = TextureArrayRHI;
    Params.LayerSampler = TStaticSamplerState<SF_Trilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
    Params.OutputTexture = OutputUAV;

    const uint32 GroupX = (static_cast<uint32>(NumChunks) + 7u) / 8u;

    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
