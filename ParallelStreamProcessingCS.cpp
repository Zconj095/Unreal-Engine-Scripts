// Implementation of ParallelStreamProcessing.usf dispatch helper

#include "ParallelStreamProcessingCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FParallelStreamProcessingCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FParallelStreamProcessingCS);
    SHADER_USE_PARAMETER_STRUCT(FParallelStreamProcessingCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER_SRV(StructuredBuffer<float4>, InterleavedBuffer)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float4>, ProcessedStream)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FParallelStreamProcessingCS, "/PEKBGGAP/ParallelStreamProcessing.usf", "ProcessStream", SF_Compute);

void DispatchParallelStreamProcessing(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* InSRV,
    FRHIUnorderedAccessView* OutUAV,
    uint32 NumElements)
{
    check(InSRV != nullptr);
    check(OutUAV != nullptr);

    TShaderMapRef<FParallelStreamProcessingCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FParallelStreamProcessingCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.InterleavedBuffer = InSRV;
    Params.ProcessedStream = OutUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
