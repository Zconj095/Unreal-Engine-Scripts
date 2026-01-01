// Unreal conversion of Unity MemoryProcessingSpeed using a minimal MLP

#include "MemoryProcessingSpeedActor.h"
#include "MemoryProcessingSpeedManager.h"
#include "HAL/PlatformProcess.h"

AMemoryProcessingSpeedActor::AMemoryProcessingSpeedActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

static void SimulateProcessing(const TArray<double>& Features)
{
    // Simple CPU work similar to Unity example: sum of squares
    volatile double Accum = 0.0;
    for (double v : Features)
    {
        Accum += v * v;
    }
}

static void AddProcessingTimer(UMemoryProcessingSpeedManager* Manager, const TArray<double>& Features)
{
    const double Start = FPlatformTime::Seconds();
    SimulateProcessing(Features);
    const double End = FPlatformTime::Seconds();
    const double ProcessingMicro = (End - Start) * 1.0e6; // microseconds
    Manager->AddProcessingData(Features, ProcessingMicro);

    // Log features and time
    FString FeatStr;
    for (int32 i = 0; i < Features.Num(); ++i)
    {
        if (i > 0) FeatStr += TEXT(", ");
        FeatStr += FString::SanitizeFloat(Features[i]);
    }
    UE_LOG(LogTemp, Log, TEXT("Measured Processing Time: %.3f μs for Features [%s]"), ProcessingMicro, *FeatStr);
}

void AMemoryProcessingSpeedActor::BeginPlay()
{
    Super::BeginPlay();

    // Create manager
    UMemoryProcessingSpeedManager* Manager = NewObject<UMemoryProcessingSpeedManager>(this);
    Manager->InitializeNetwork(/*inputSize*/ 3, /*hiddenNeurons*/ 10, /*outputSize*/ 1);

    // Simulate processing measurements
    AddProcessingTimer(Manager, TArray<double>({1.0, 2.0, 3.0}));
    AddProcessingTimer(Manager, TArray<double>({2.0, 3.0, 4.0}));
    AddProcessingTimer(Manager, TArray<double>({3.0, 4.0, 5.0}));

    // Train neural network
    const double FinalError = Manager->TrainNetwork(/*epochs*/ 1000, /*lr*/ 0.01);
    UE_LOG(LogTemp, Log, TEXT("Final Training Error: %f"), FinalError);

    // Average processing time
    const double AvgMicro = Manager->CalculateAverageProcessingTime();
    UE_LOG(LogTemp, Log, TEXT("Average Memory Processing Time: %.3f μs"), AvgMicro);

    // Predict
    const TArray<double> TestFeatures = { 4.0, 5.0, 6.0 };
    const double PredictedMicro = Manager->PredictProcessingTime(TestFeatures);
    UE_LOG(LogTemp, Log, TEXT("Predicted Processing Time for Features [4, 5, 6]: %.3f μs"), PredictedMicro);
}

