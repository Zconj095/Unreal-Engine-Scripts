#include "FractalDatabase.h"

#include "EmbeddingSimilarity.h"

void FFractalDatabase::AddPattern(const FEmbedding& Embedding)
{
    FractalPatterns.Add(Embedding);
}

float FFractalDatabase::ComputeFractalDistortion(const FEmbedding& Embedding) const
{
    const int32 N = FractalPatterns.Num();
    if (N <= 0)
    {
        return 1.0f; // No patterns to compare; maximum distortion
    }

    float DistortionScore = 0.f;
    for (const FEmbedding& Pattern : FractalPatterns)
    {
        DistortionScore += FEmbeddingSimilarity::ComputeCosineSimilarity(Embedding, Pattern);
    }

    const float Avg = DistortionScore / static_cast<float>(N);
    return 1.0f - Avg;
}

