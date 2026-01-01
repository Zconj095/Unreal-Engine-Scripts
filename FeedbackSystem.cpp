#include "FeedbackSystem.h"

TArray<float> FFeedbackSystem::ApplyFeedback(const TArray<float>& Vector,
                                             const TArray<float>& Feedback,
                                             float LearningRate)
{
    const int32 N = FMath::Min(Vector.Num(), Feedback.Num());
    TArray<float> Updated;
    Updated.SetNumUninitialized(N);
    for (int32 i = 0; i < N; ++i)
    {
        Updated[i] = Vector[i] + LearningRate * Feedback[i];
    }
    return Updated;
}

TArray<float> FFeedbackSystem::GenerateFeedback(const TArray<float>& Source,
                                                const TArray<float>& Target,
                                                TFunctionRef<float(float, float)> FeedbackFunction)
{
    const int32 N = FMath::Min(Source.Num(), Target.Num());
    TArray<float> Out;
    Out.SetNumUninitialized(N);
    for (int32 i = 0; i < N; ++i)
    {
        Out[i] = FeedbackFunction(Source[i], Target[i]);
    }
    return Out;
}

