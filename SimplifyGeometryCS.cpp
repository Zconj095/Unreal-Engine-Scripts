// Implementation of SimplifyGeometry.usf dispatch helper

#include "SimplifyGeometryCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FSimplifyGeometryCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FSimplifyGeometryCS);
    SHADER_USE_PARAMETER_STRUCT(FSimplifyGeometryCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER(float, ScaleFactor)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, OriginalVertices)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float3>, SimplifiedVertices)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FSimplifyGeometryCS, "/PEKBGGAP/SimplifyGeometry.usf", "SimplifyGeometry", SF_Compute);

void DispatchSimplifyGeometry(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* InSRV,
    FRHIUnorderedAccessView* OutUAV,
    uint32 NumElements,
    float ScaleFactor)
{
    check(InSRV != nullptr);
    check(OutUAV != nullptr);

    TShaderMapRef<FSimplifyGeometryCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FSimplifyGeometryCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.ScaleFactor = ScaleFactor;
    Params.OriginalVertices = InSRV;
    Params.SimplifiedVertices = OutUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        ComputeShader,
        Params,
        FIntVector(GroupX, 1, 1));
}
