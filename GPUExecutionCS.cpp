// Implementation of GPUExecution.usf dispatch helper

#include "GPUExecutionCS.h"

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "RenderGraphUtils.h"

class FGPUExecutionCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FGPUExecutionCS);
    SHADER_USE_PARAMETER_STRUCT(FGPUExecutionCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(int32, Resolution)
        SHADER_PARAMETER(float, Frequency)
        SHADER_PARAMETER(int32, Octaves)
        SHADER_PARAMETER(float, Lacunarity)
        SHADER_PARAMETER(float, Gain)
        SHADER_PARAMETER_UAV(RWTexture2D<float>, HeightMap)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Params)
    {
        return true;
    }
};

IMPLEMENT_GLOBAL_SHADER(FGPUExecutionCS, "/PEKBGGAP/GPUExecution.usf", "CSMain", SF_Compute);

void DispatchGPUHeightMap(
    FRHICommandListImmediate& RHICmdList,
    FRHIUnorderedAccessView* HeightMapUAV,
    int32 Resolution,
    float Frequency,
    int32 Octaves,
    float Lacunarity,
    float Gain)
{
    check(HeightMapUAV != nullptr);

    TShaderMapRef<FGPUExecutionCS> CS(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    FGPUExecutionCS::FParameters Params;
    Params.Resolution = Resolution;
    Params.Frequency = Frequency;
    Params.Octaves = Octaves;
    Params.Lacunarity = Lacunarity;
    Params.Gain = Gain;
    Params.HeightMap = HeightMapUAV;

    const uint32 GroupX = (Resolution + 7u) / 8u;
    const uint32 GroupY = (Resolution + 7u) / 8u;

    FComputeShaderUtils::Dispatch(
        RHICmdList,
        CS,
        Params,
        FIntVector(GroupX, GroupY, 1));
}
