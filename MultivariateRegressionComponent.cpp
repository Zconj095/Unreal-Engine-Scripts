#include "MultivariateRegressionComponent.h"

#include "Containers/Array.h"

namespace
{
    class FSimpleLinearRegression
    {
    public:
        void Fit(const TArray<TArray<float>>& X, const TArray<float>& Y) const
        {
            UE_LOG(LogTemp, Log, TEXT("Fit model here"));
        }
    };
}

UMultivariateRegressionComponent::UMultivariateRegressionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMultivariateRegressionComponent::BeginPlay()
{
    Super::BeginPlay();

    // Unity Start(): example data and call Fit
    TArray<TArray<float>> X;
    X.Add({1.f, 2.f});
    X.Add({3.f, 1.f});

    TArray<float> Y;
    Y.Add(3.f);
    Y.Add(5.f);

    FSimpleLinearRegression Reg;
    Reg.Fit(X, Y);
}

