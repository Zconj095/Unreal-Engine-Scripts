// Implementation of ParallelProcessingonGPU.usf dispatch helper

#include "ParallelProcessingonGPUCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FParallelProcessingonGPUCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FParallelProcessingonGPUCS);
    SHADER_USE_PARAMETER_STRUCT(FParallelProcessingonGPUCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER(float, LodScale)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, AlignedStreamPosition)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, AlignedStreamNormal)
        SHADER_PARAMETER_SRV(StructuredBuffer<int>,    AlignedStreamLOD)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float3>, ProcessedPosition)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FParallelProcessingonGPUCS, "/PEKBGGAP/ParallelProcessingonGPU.usf", "ProcessAlignedStream", SF_Compute);

void DispatchParallelProcessingOnGPU(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* PosSRV,
    FRHIShaderResourceView* NormalSRV,
    FRHIShaderResourceView* LODSRV,
    FRHIUnorderedAccessView* OutUAV,
    uint32 NumElements,
    float LodScale)
{
    check(PosSRV != nullptr);
    check(NormalSRV != nullptr);
    check(LODSRV != nullptr);
    check(OutUAV != nullptr);

    TShaderMapRef<FParallelProcessingonGPUCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FParallelProcessingonGPUCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.LodScale = LodScale;
    Params.AlignedStreamPosition = PosSRV;
    Params.AlignedStreamNormal = NormalSRV;
    Params.AlignedStreamLOD = LODSRV;
    Params.ProcessedPosition = OutUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(RHICmdList, CS, Params, FIntVector(GroupX, 1, 1));
}
