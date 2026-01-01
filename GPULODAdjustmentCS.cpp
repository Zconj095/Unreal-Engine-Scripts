// Implementation of GPULODAdjustment.usf dispatch helper

#include "GPULODAdjustmentCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FGPULODAdjustmentCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FGPULODAdjustmentCS);
    SHADER_USE_PARAMETER_STRUCT(FGPULODAdjustmentCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER(uint32, ChunkSize)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, RawVertices)
        SHADER_PARAMETER_SRV(StructuredBuffer<int>, LODLevels)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float3>, AdjustedVertices)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FGPULODAdjustmentCS, "/PEKBGGAP/GPULODAdjustment.usf", "CSMain", SF_Compute);

void DispatchGPULODAdjustment(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* RawVerticesSRV,
    FRHIShaderResourceView* LODLevelsSRV,
    FRHIUnorderedAccessView* AdjustedVerticesUAV,
    uint32 NumElements,
    uint32 ChunkSize)
{
    check(RawVerticesSRV != nullptr);
    check(LODLevelsSRV != nullptr);
    check(AdjustedVerticesUAV != nullptr);

    TShaderMapRef<FGPULODAdjustmentCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FGPULODAdjustmentCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.ChunkSize = ChunkSize;
    Params.RawVertices = RawVerticesSRV;
    Params.LODLevels = LODLevelsSRV;
    Params.AdjustedVertices = AdjustedVerticesUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
