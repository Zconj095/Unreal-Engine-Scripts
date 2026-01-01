#include "MagicalProcessorActor.h"
#include "Math/UnrealMathUtility.h"

AMagicalProcessorActor::AMagicalProcessorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMagicalProcessorActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<float> Linear;
	InitializeSylvanCortex(Linear);

	FString LinearString;
	for (float Value : Linear)
	{
		LinearString += FString::Printf(TEXT("%.3f "), Value);
	}
	UE_LOG(LogTemp, Log, TEXT("Sylvan Cortex M_spell: %s"), *LinearString);

	InitializeAetherialLobe();
	ComputeRunicCore(5);
}

void AMagicalProcessorActor::InitializeSylvanCortex(TArray<float>& OutLinear)
{
	OutLinear.SetNumZeroed(SylvanOutputSize);
	for (int32 Index = 0; Index < OutLinear.Num(); ++Index)
	{
		const float Value = FMath::FRand();
		OutLinear[Index] = Sigmoid(Value);
	}
}

float AMagicalProcessorActor::Sigmoid(float Value) const
{
	return 1.f / (1.f + FMath::Exp(-Value));
}

void AMagicalProcessorActor::InitializeAetherialLobe()
{
	TArray<float> Amplitudes;
	Amplitudes.Init(1.f / AetherialThreads, AetherialThreads);

	TArray<float> BrainState;
	BrainState.Init(0.f, ThreadStateSize);

	for (int32 Thread = 0; Thread < AetherialThreads; ++Thread)
	{
		for (int32 Dimension = 0; Dimension < ThreadStateSize; ++Dimension)
		{
			BrainState[Dimension] += Amplitudes[Thread] * FMath::FRand();
		}
	}

	FString BrainStateStr;
	for (float Value : BrainState)
	{
		BrainStateStr += FString::Printf(TEXT("%.3f "), Value);
	}
	UE_LOG(LogTemp, Log, TEXT("Aetherial Lobe Brain State |Ψ⟩: %s"), *BrainStateStr);
}

void AMagicalProcessorActor::ComputeRunicCore(int32 N) const
{
	int32 Value = N;
	int32 Result = 1;
	while (Value > 0)
	{
		Result = 2 * Result + 3;
		--Value;
	}
	UE_LOG(LogTemp, Log, TEXT("Runic Core R(%d): %d"), N, Result);
}
