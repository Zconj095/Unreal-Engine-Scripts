// Implementation of GPURefinement.usf dispatch helper

#include "GPURefinementCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FGPURefinementCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FGPURefinementCS);
    SHADER_USE_PARAMETER_STRUCT(FGPURefinementCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(int32, Resolution)
        SHADER_PARAMETER(float, Frequency)
        SHADER_PARAMETER_TEXTURE(Texture2D<float>, CoarseTexture)
        SHADER_PARAMETER_UAV(RWTexture2D<float>, RefinedTexture)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FGPURefinementCS, "/PEKBGGAP/GPURefinement.usf", "RefineData", SF_Compute);

void DispatchGPURefinement(
    FRHICommandListImmediate& RHICmdList,
    FRHITexture* CoarseTex,
    FRHIUnorderedAccessView* RefinedUAV,
    int32 Resolution,
    float Frequency)
{
    check(CoarseTex != nullptr);
    check(RefinedUAV != nullptr);

    TShaderMapRef<FGPURefinementCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FGPURefinementCS::FParameters Params;
    Params.Resolution = Resolution;
    Params.Frequency = Frequency;
    Params.CoarseTexture = CoarseTex;
    Params.RefinedTexture = RefinedUAV;

    const uint32 GroupX = (Resolution + 7u) / 8u;
    const uint32 GroupY = (Resolution + 7u) / 8u;

    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, GroupY, 1));
}
