#include "MagicalNeuralInteractionActor.h"
#include "Math/UnrealMathUtility.h"

AMagicalNeuralInteractionActor::AMagicalNeuralInteractionActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMagicalNeuralInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeHamiltonian();
}

void AMagicalNeuralInteractionActor::ComputeHamiltonian() const
{
	const TArray<TArray<float>> QubitStates = {
		{ 1.f, 0.f },
		{ 0.f, 1.f }
	};

	const TArray<TArray<float>> SigmaX = {
		{ 0.f, 1.f },
		{ 1.f, 0.f }
	};

	const TArray<TArray<float>> SigmaZ = {
		{ 1.f, 0.f },
		{ 0.f, -1.f }
	};

	float Hamiltonian = 0.f;

	for (int32 Row = 0; Row < QubitStates.Num(); ++Row)
	{
		for (int32 Col = 0; Col < QubitStates[Row].Num(); ++Col)
		{
			const float DotX = SigmaX[Row][0] * SigmaX[Col][0] + SigmaX[Row][1] * SigmaX[Col][1];
			const float DotZ = SigmaZ[Row][0] * SigmaZ[Col][0] + SigmaZ[Row][1] * SigmaZ[Col][1];
			Hamiltonian += -CouplingStrength * (DotX + DotZ);
		}

		const float NormZ = FMath::Sqrt(
			SigmaZ[Row][0] * SigmaZ[Row][0] + SigmaZ[Row][1] * SigmaZ[Row][1]);
		Hamiltonian -= ResonanceField * NormZ;
	}

	UE_LOG(LogTemp, Log, TEXT("Effective Hamiltonian: %f"), Hamiltonian);
}
