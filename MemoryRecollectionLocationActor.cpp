// Unreal conversion of Unity MemoryRecollectionLocation with simple 2-output MLP

#include "MemoryRecollectionLocationActor.h"
#include "MemoryRecollectionLocationManager.h"

AMemoryRecollectionLocationActor::AMemoryRecollectionLocationActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMemoryRecollectionLocationActor::BeginPlay()
{
    Super::BeginPlay();

    UMemoryRecollectionLocationManager* Manager = NewObject<UMemoryRecollectionLocationManager>(this);
    Manager->InitializeNeuralNetwork(/*inputSize*/ 3, /*hiddenNeurons*/ 5, /*outputSize*/ 2);

    // Add three entries (mirrors Unity sample)
    Manager->AddRecollectionEntry(TEXT("Entry1"), FVector2D(2.0f, 3.0f), TArray<double>({1.0, 2.0, 3.0}), 1.5f);
    Manager->AddRecollectionEntry(TEXT("Entry2"), FVector2D(4.0f, 5.0f), TArray<double>({2.0, 3.0, 4.0}), 2.0f);
    Manager->AddRecollectionEntry(TEXT("Entry3"), FVector2D(6.0f, 7.0f), TArray<double>({4.0, 5.0, 6.0}), 0.8f);

    const double AvgTime = Manager->CalculateAverageRecollectionTime();
    UE_LOG(LogTemp, Log, TEXT("Average Recollection Time: %f seconds"), AvgTime);

    const FVector2D AvgLoc = Manager->CalculateAverageLocation();
    UE_LOG(LogTemp, Log, TEXT("Average Recollection Location: (%f, %f)"), AvgLoc.X, AvgLoc.Y);

    // Train NN
    Manager->TrainNetwork(/*epochs*/ 1000, /*lr*/ 0.01);

    // Predict
    const FVector2D Pred = Manager->PredictRecollectionLocation(TArray<double>({3.0, 4.0, 5.0}));
    UE_LOG(LogTemp, Log, TEXT("Predicted Recollection Location for Features [3, 4, 5]: (%f, %f)"), Pred.X, Pred.Y);
}

