// Implementation of ParallelExecutionExample.usf dispatch helper

#include "ParallelExecutionExampleCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FParallelExecutionExampleCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FParallelExecutionExampleCS);
    SHADER_USE_PARAMETER_STRUCT(FParallelExecutionExampleCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER(float, Scale)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, StreamPositions)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float3>, ProcessedPositions)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FParallelExecutionExampleCS, "/PEKBGGAP/ParallelExecutionExample.usf", "ExecuteStream", SF_Compute);

void DispatchParallelExecutionExample(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* StreamPositionsSRV,
    FRHIUnorderedAccessView* ProcessedPositionsUAV,
    uint32 NumElements,
    float Scale)
{
    check(StreamPositionsSRV != nullptr);
    check(ProcessedPositionsUAV != nullptr);

    TShaderMapRef<FParallelExecutionExampleCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FParallelExecutionExampleCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.Scale = Scale;
    Params.StreamPositions = StreamPositionsSRV;
    Params.ProcessedPositions = ProcessedPositionsUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
