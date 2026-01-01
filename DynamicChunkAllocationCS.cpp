// Implementation of DynamicChunkAllocation.usf dispatch helper

#include "DynamicChunkAllocationCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FDynamicChunkAllocationCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FDynamicChunkAllocationCS);
    SHADER_USE_PARAMETER_STRUCT(FDynamicChunkAllocationCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(int32, ChunkCount)
        SHADER_PARAMETER_SRV(StructuredBuffer<int>, VisibilityFlags)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<int>, ChunkAllocation)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FDynamicChunkAllocationCS, "/PEKBGGAP/DynamicChunkAllocation.usf", "AllocateChunks", SF_Compute);

void DispatchDynamicChunkAllocation(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* VisibilityFlagsSRV,
    FRHIUnorderedAccessView* ChunkAllocationUAV,
    uint32 ChunkCount)
{
    check(VisibilityFlagsSRV != nullptr);
    check(ChunkAllocationUAV != nullptr);

    TShaderMapRef<FDynamicChunkAllocationCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FDynamicChunkAllocationCS::FParameters Params;
    Params.ChunkCount = static_cast<int32>(ChunkCount);
    Params.VisibilityFlags = VisibilityFlagsSRV;
    Params.ChunkAllocation = ChunkAllocationUAV;

    const uint32 GroupX = (ChunkCount + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
