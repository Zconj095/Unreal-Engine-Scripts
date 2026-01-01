#include "LeyFieldTensorFlowActor.h"
#include "Engine/Engine.h"

ALeyFieldTensorFlowActor::ALeyFieldTensorFlowActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeyFieldTensorFlowActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<double> PhiInput = { 1.0, 2.0, 3.0 };
	TArray<double> LeyInput = { 4.0, 5.0, 6.0 };

	if (PhiInput.Num() != LeyInput.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("TensorFlow inputs must match sizes."));
		return;
	}

	double DotProduct = 0.0;
	for (int32 Index = 0; Index < PhiInput.Num(); ++Index)
	{
		DotProduct += PhiInput[Index] * LeyInput[Index];
	}

	UE_LOG(LogTemp, Log, TEXT("TensorFlow Dot Product Result: %.0f"), DotProduct);
}
