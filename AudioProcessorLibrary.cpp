#include "AudioProcessorLibrary.h"

void UAudioProcessorLibrary::ConvertAudioToEmbedding(const TArray<uint8>& AudioData, TArray<float>& OutEmbedding, int32 EmbeddingDim)
{
    if (EmbeddingDim <= 0)
    {
        OutEmbedding.Reset();
        return;
    }

    OutEmbedding.SetNumUninitialized(EmbeddingDim);

    if (AudioData.Num() == 0)
    {
        for (int32 i = 0; i < EmbeddingDim; ++i)
        {
            OutEmbedding[i] = 0.0f;
        }
        return;
    }

    for (int32 i = 0; i < EmbeddingDim; ++i)
    {
        const uint8 Byte = AudioData[i % AudioData.Num()];
        OutEmbedding[i] = static_cast<float>(Byte) / 255.0f;
    }
}

