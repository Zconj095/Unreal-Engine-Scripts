// Implementation of DrivenUpdates.usf dispatch helper

#include "DrivenUpdatesCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FDrivenUpdatesCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FDrivenUpdatesCS);
    SHADER_USE_PARAMETER_STRUCT(FDrivenUpdatesCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(uint32, NumElements)
        SHADER_PARAMETER_SRV(StructuredBuffer<float3>, OldValues)
        SHADER_PARAMETER_UAV(RWStructuredBuffer<float3>, NewValues)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FDrivenUpdatesCS, "/PEKBGGAP/DrivenUpdates.usf", "GPUUpdate", SF_Compute);

void DispatchDrivenUpdates(
    FRHICommandListImmediate& RHICmdList,
    FRHIShaderResourceView* OldValuesSRV,
    FRHIUnorderedAccessView* NewValuesUAV,
    uint32 NumElements)
{
    check(OldValuesSRV != nullptr);
    check(NewValuesUAV != nullptr);

    TShaderMapRef<FDrivenUpdatesCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FDrivenUpdatesCS::FParameters Params;
    Params.NumElements = NumElements;
    Params.OldValues = OldValuesSRV;
    Params.NewValues = NewValuesUAV;

    const uint32 GroupX = (NumElements + 256u - 1u) / 256u;
    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, 1, 1));
}
