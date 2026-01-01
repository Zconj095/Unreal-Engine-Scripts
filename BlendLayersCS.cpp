// Implementation of the BlendLayers.usf dispatch helper

#include "BlendLayersCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FBlendLayersCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FBlendLayersCS);
    SHADER_USE_PARAMETER_STRUCT(FBlendLayersCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(int32, Resolution)
        SHADER_PARAMETER(int32, NumLayers)
        SHADER_PARAMETER_SRV(StructuredBuffer<float4>, TextureWeights)
        SHADER_PARAMETER_TEXTURE(Texture2DArray<float4>, LayerTextures)
        SHADER_PARAMETER_SAMPLER(SamplerState, LayerSampler)
        SHADER_PARAMETER_UAV(RWTexture2D<float4>, BlendedTexture)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FBlendLayersCS, "/PEKBGGAP/BlendLayers.usf", "CSMain", SF_Compute);

void DispatchBlendLayers(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* WeightSRV,
    FRHITexture* LayerTexArray,
    FRHIUnorderedAccessView* OutBlendedUAV,
    int32 Resolution,
    int32 NumLayers)
{
    check(WeightSRV != nullptr);
    check(LayerTexArray != nullptr);
    check(OutBlendedUAV != nullptr);

    TShaderMapRef<FBlendLayersCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FBlendLayersCS::FParameters Params;
    Params.Resolution = Resolution;
    Params.NumLayers = NumLayers;
    Params.TextureWeights = WeightSRV;
    Params.LayerTextures = LayerTexArray;
    Params.LayerSampler = TStaticSamplerState<SF_Trilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
    Params.BlendedTexture = OutBlendedUAV;

    const uint32 GroupX = (Resolution + 7u) / 8u;
    const uint32 GroupY = (Resolution + 7u) / 8u;

    FComputeShaderUtils::Dispatch(
        RHICmdList,
        ComputeShader,
        Params,
        FIntVector(GroupX, GroupY, 1));
}
