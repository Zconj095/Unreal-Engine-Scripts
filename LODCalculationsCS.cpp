// Implementation of LODCalculations.usf dispatch helper

#include "LODCalculationsCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FLODCalculationsCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FLODCalculationsCS);
    SHADER_USE_PARAMETER_STRUCT(FLODCalculationsCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER_SRV(StructuredBuffer<float4>, VertexLODBuffer)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float4>, ProcessedVertexBuffer)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FLODCalculationsCS, "/PEKBGGAP/LODCalculations.usf", "ComputeLOD", SF_Compute);

void DispatchLODCalculations(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* InSRV,
    FRHIUnorderedAccessView* OutUAV,
    uint32 NumElements)
{
    check(InSRV != nullptr);
    check(OutUAV != nullptr);

    TShaderMapRef<FLODCalculationsCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FLODCalculationsCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.VertexLODBuffer = InSRV;
    Params.ProcessedVertexBuffer = OutUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
