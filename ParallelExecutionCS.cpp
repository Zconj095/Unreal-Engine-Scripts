// Implementation of ParallelExecution.usf dispatch helper

#include "ParallelExecutionCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FParallelExecutionCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FParallelExecutionCS);
    SHADER_USE_PARAMETER_STRUCT(FParallelExecutionCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER(float, DeltaTime)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, Positions)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, Velocities)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float3>, UpdatedPositions)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FParallelExecutionCS, "/PEKBGGAP/ParallelExecution.usf", "UpdateStreams", SF_Compute);

void DispatchParallelExecution(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* PositionsSRV,
    FRHIShaderResourceView* VelocitiesSRV,
    FRHIUnorderedAccessView* UpdatedPositionsUAV,
    uint32 NumElements,
    float DeltaTime)
{
    check(PositionsSRV != nullptr);
    check(VelocitiesSRV != nullptr);
    check(UpdatedPositionsUAV != nullptr);

    TShaderMapRef<FParallelExecutionCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FParallelExecutionCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.DeltaTime = DeltaTime;
    Params.Positions = PositionsSRV;
    Params.Velocities = VelocitiesSRV;
    Params.UpdatedPositions = UpdatedPositionsUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
