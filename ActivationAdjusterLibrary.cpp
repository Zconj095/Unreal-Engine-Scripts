#include "ActivationAdjusterLibrary.h"
#include "Math/UnrealMathUtility.h"

float UActivationAdjusterLibrary::AdjustReLU(float X, float Distortion)
{
    return FMath::Max(0.0f, X - Distortion);
}

float UActivationAdjusterLibrary::AdjustSigmoid(float X, float Distortion)
{
    const float Sigmoid = 1.0f / (1.0f + static_cast<float>(FMath::Exp(-X)));
    return Sigmoid * (1.0f - Distortion);
}

