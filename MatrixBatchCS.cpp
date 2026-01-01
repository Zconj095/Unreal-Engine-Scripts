// Implementation of MatrixBatch.usf dispatch helper

#include "MatrixBatchCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FMatrixBatchCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FMatrixBatchCS);
    SHADER_USE_PARAMETER_STRUCT(FMatrixBatchCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FMatrix44f, TransformationMatrix)
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER_SRV(StructuredBuffer<float4>, RawVertices)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float4>, TransformedVertices)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FMatrixBatchCS, "/PEKBGGAP/MatrixBatch.usf", "CSMain", SF_Compute);

void DispatchMatrixBatch(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* InSRV,
    FRHIUnorderedAccessView* OutUAV,
    const FMatrix44f& Transform,
    uint32 NumElements)
{
    check(InSRV != nullptr);
    check(OutUAV != nullptr);

    TShaderMapRef<FMatrixBatchCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FMatrixBatchCS::FParameters Params;
    Params.TransformationMatrix = Transform;
    Params.NumElements = NumElements;
    Params.RawVertices = InSRV;
    Params.TransformedVertices = OutUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
