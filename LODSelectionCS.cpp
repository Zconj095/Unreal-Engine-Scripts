// Implementation of LODSelection.usf dispatch helper

#include "LODSelectionCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FLODSelectionCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FLODSelectionCS);
    SHADER_USE_PARAMETER_STRUCT(FLODSelectionCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER(float, HighThreshold)
        SHADER_PARAMETER(float, MediumThreshold)
        SHADER_PARAMETER_SRV(StructuredBuffer<float>, Distances)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<int>, LODs)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FLODSelectionCS, "/PEKBGGAP/LODSelection.usf", "LODSelection", SF_Compute);

void DispatchLODSelection(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* DistancesSRV,
    FRHIUnorderedAccessView* LODsUAV,
    uint32 NumElements,
    float HighThreshold,
    float MediumThreshold)
{
    check(DistancesSRV != nullptr);
    check(LODsUAV != nullptr);

    TShaderMapRef<FLODSelectionCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FLODSelectionCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.HighThreshold = HighThreshold;
    Params.MediumThreshold = MediumThreshold;
    Params.Distances = DistancesSRV;
    Params.LODs = LODsUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
