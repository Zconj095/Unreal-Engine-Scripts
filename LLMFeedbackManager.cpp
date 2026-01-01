#include "LLMFeedbackManager.h"

void ULLMFeedbackManager::RecordFeedback(const FString& Response, float Score)
{
    if (float* Existing = FeedbackScores.Find(Response))
    {
        *Existing = (*Existing + Score) * 0.5f; // average
    }
    else
    {
        FeedbackScores.Add(Response, Score);
    }
}

float ULLMFeedbackManager::GetFeedbackScore(const FString& Response) const
{
    if (const float* Found = FeedbackScores.Find(Response))
    {
        return *Found;
    }
    return 0.5f; // neutral default
}

