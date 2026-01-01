// Implementation of LocalNoiseRefinement.usf dispatch helper

#include "LocalNoiseRefinementCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FLocalNoiseRefinementCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FLocalNoiseRefinementCS);
    SHADER_USE_PARAMETER_STRUCT(FLocalNoiseRefinementCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(int32, Resolution)
        SHADER_PARAMETER(float, LocalScale)
        SHADER_PARAMETER_TEXTURE(Texture2D<float>, GlobalNoise)
        SHADER_PARAMETER_UAV(RWTexture2D<float>, RefinedNoise)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FLocalNoiseRefinementCS, "/PEKBGGAP/LocalNoiseRefinement.usf", "CSMain", SF_Compute);

void DispatchLocalNoiseRefinement(
    FRHICommandListImmediate& RHICmdList,
    FRHITexture* GlobalNoiseTex,
    FRHIUnorderedAccessView* RefinedNoiseUAV,
    int32 Resolution,
    float LocalScale)
{
    check(GlobalNoiseTex != nullptr);
    check(RefinedNoiseUAV != nullptr);

    TShaderMapRef<FLocalNoiseRefinementCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FLocalNoiseRefinementCS::FParameters Params;
    Params.Resolution = Resolution;
    Params.LocalScale = LocalScale;
    Params.GlobalNoise = GlobalNoiseTex;
    Params.RefinedNoise = RefinedNoiseUAV;

    const uint32 GroupX = (Resolution + 7u) / 8u;
    const uint32 GroupY = (Resolution + 7u) / 8u;

    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, GroupY, 1));
}
