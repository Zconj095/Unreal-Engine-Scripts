// Implementation of GeometrySimplification.usf dispatch helper

#include "GeometrySimplificationCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FGeometrySimplificationCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FGeometrySimplificationCS);
    SHADER_USE_PARAMETER_STRUCT(FGeometrySimplificationCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER(float, WeightNormals)
        SHADER_PARAMETER(float, WeightVolume)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, OriginalVertices)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float3>, ReducedVertices)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FGeometrySimplificationCS, "/PEKBGGAP/GeometrySimplification.usf", "SimplifyGeometry", SF_Compute);

void DispatchGeometrySimplification(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* OriginalVerticesSRV,
    FRHIUnorderedAccessView* ReducedVerticesUAV,
    uint32 NumElements,
    float WeightNormals,
    float WeightVolume)
{
    check(OriginalVerticesSRV != nullptr);
    check(ReducedVerticesUAV != nullptr);

    TShaderMapRef<FGeometrySimplificationCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FGeometrySimplificationCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.WeightNormals = WeightNormals;
    Params.WeightVolume = WeightVolume;
    Params.OriginalVertices = OriginalVerticesSRV;
    Params.ReducedVertices = ReducedVerticesUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
