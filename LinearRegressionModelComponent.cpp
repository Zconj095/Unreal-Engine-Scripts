#include "LinearRegressionModelComponent.h"

#include "Math/UnrealMathUtility.h"

ULinearRegressionModelComponent::ULinearRegressionModelComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULinearRegressionModelComponent::Fit(const TArray<TArray<float>>& X, const TArray<float>& Y)
{
    const int32 M = Y.Num();
    if (M <= 0 || X.Num() != M || X[0].Num() <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("LinearRegression Fit: invalid input sizes"));
        Slopes.Reset();
        Intercept = 0.f;
        return;
    }

    const int32 N = X[0].Num();
    for (int32 i = 1; i < M; ++i)
    {
        if (X[i].Num() != N)
        {
            UE_LOG(LogTemp, Warning, TEXT("LinearRegression Fit: non-rectangular X"));
            Slopes.Reset();
            Intercept = 0.f;
            return;
        }
    }

    // Means
    float YBar = 0.f;
    for (int32 i = 0; i < M; ++i) { YBar += Y[i]; }
    YBar /= static_cast<float>(M);

    TArray<float> XBar; XBar.Init(0.f, N);
    for (int32 j = 0; j < N; ++j)
    {
        float Sum = 0.f;
        for (int32 i = 0; i < M; ++i) { Sum += X[i][j]; }
        XBar[j] = Sum / static_cast<float>(M);
    }

    // Slopes per feature
    Slopes.Init(0.f, N);
    for (int32 j = 0; j < N; ++j)
    {
        float Num = 0.f;
        float Den = 0.f;
        const float XBarJ = XBar[j];
        for (int32 i = 0; i < M; ++i)
        {
            const float Xc = X[i][j] - XBarJ;
            const float Yc = Y[i] - YBar;
            Num += Xc * Yc;
            Den += Xc * Xc;
        }
        Slopes[j] = (Den != 0.f) ? (Num / Den) : 0.f;
    }

    // Intercept = yBar - sum_j (B_j * xBar_j)
    float BxBar = 0.f;
    for (int32 j = 0; j < N; ++j)
    {
        BxBar += Slopes[j] * XBar[j];
    }
    Intercept = YBar - BxBar;

    UE_LOG(LogTemp, Log, TEXT("Regression Fitted with Intercept: %f"), Intercept);
}

