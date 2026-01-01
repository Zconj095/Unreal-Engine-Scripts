// Implementation of FrustumCulling.usf dispatch helper

#include "FrustumCullingCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FFrustumCullingCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FFrustumCullingCS);
    SHADER_USE_PARAMETER_STRUCT(FFrustumCullingCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER_ARRAY(FVector4f, FrustumPlanes, [6])
        SHADER_PARAMETER_SRV(StructuredBuffer<float4>, ChunkBounds)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<int>, VisibilityFlags)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FFrustumCullingCS, "/PEKBGGAP/FrustumCulling.usf", "CSMain", SF_Compute);

void DispatchFrustumCulling(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* ChunkBoundsSRV,
    FRHIUnorderedAccessView* VisibilityFlagsUAV,
    const FVector4f* InFrustumPlanes,
    uint32 NumElements)
{
    check(ChunkBoundsSRV != nullptr);
    check(VisibilityFlagsUAV != nullptr);
    check(InFrustumPlanes != nullptr);

    TShaderMapRef<FFrustumCullingCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FFrustumCullingCS::FParameters Params;
    Params.NumElements = NumElements;
    // Copy 6 planes
    for (int32 i = 0; i < 6; ++i)
    {
        Params.FrustumPlanes[i] = InFrustumPlanes[i];
    }
    Params.ChunkBounds = ChunkBoundsSRV;
    Params.VisibilityFlags = VisibilityFlagsUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
