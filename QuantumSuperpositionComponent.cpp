#include "QuantumSuperpositionComponent.h"

#include "InputCoreTypes.h"
#include "Logging/LogMacros.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

UQuantumSuperpositionComponent::UQuantumSuperpositionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuantumSuperpositionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (States.Num() == 0)
	{
		States = {
			{TEXT("State A"), UComplexNumberLibrary::Create(0.5, 0.5)},
			{TEXT("State B"), UComplexNumberLibrary::Create(0.5, -0.5)},
			{TEXT("State C"), UComplexNumberLibrary::Create(0.5, 0.5)},
			{TEXT("State D"), UComplexNumberLibrary::Create(0.5, -0.5)}
		};
	}

	NormalizeStateVector();
	InitializeUnitaryMatrix();
}

void UQuantumSuperpositionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		if (PC->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			Collapse();
		}
		else if (PC->WasInputKeyJustPressed(EKeys::T))
		{
			ApplyUnitaryTransformation();
		}
	}
}

void UQuantumSuperpositionComponent::InitializeUnitaryMatrix()
{
	UnitaryMatrix.SetNum(16);
	auto S = [&](int32 R, int32 C, double Re, double Im)
	{
		UnitaryMatrix[R * 4 + C] = UComplexNumberLibrary::Create(Re, Im);
	};

	S(0, 0, 0.5, 0); S(0, 1, 0.5, 0); S(0, 2, 0.5, 0); S(0, 3, 0.5, 0);
	S(1, 0, 0.5, 0); S(1, 1, -0.5, 0); S(1, 2, 0.5, 0); S(1, 3, -0.5, 0);
	S(2, 0, 0.5, 0); S(2, 1, 0.5, 0); S(2, 2, -0.5, 0); S(2, 3, -0.5, 0);
	S(3, 0, 0.5, 0); S(3, 1, -0.5, 0); S(3, 2, -0.5, 0); S(3, 3, 0.5, 0);
}

void UQuantumSuperpositionComponent::NormalizeStateVector()
{
	double TotalAmpSq = 0.0;
	for (const FQuantumState& State : States)
	{
		const double Mag = State.Amplitude.GetMagnitude();
		TotalAmpSq += Mag * Mag;
	}

	const double Norm = FMath::Sqrt(TotalAmpSq);
	if (Norm <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	for (FQuantumState& State : States)
	{
		State.Amplitude = UComplexNumberLibrary::DivideByScalar(State.Amplitude, Norm);
	}
}

void UQuantumSuperpositionComponent::ApplyUnitaryTransformation()
{
	if (States.Num() != 4 || UnitaryMatrix.Num() != 16)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyUnitaryTransformation expects 4 states and a 4x4 matrix."));
		return;
	}

	TArray<FComplexNumber> NewAmplitudes;
	NewAmplitudes.SetNum(States.Num());

	for (int32 I = 0; I < States.Num(); ++I)
	{
		FComplexNumber Sum = UComplexNumberLibrary::Create(0, 0);
		for (int32 J = 0; J < States.Num(); ++J)
		{
			const FComplexNumber Prod = UComplexNumberLibrary::Multiply(UnitaryMatrix[I * 4 + J], States[J].Amplitude);
			Sum = UComplexNumberLibrary::Add(Sum, Prod);
		}
		NewAmplitudes[I] = Sum;
	}

	for (int32 I = 0; I < States.Num(); ++I)
	{
		States[I].Amplitude = NewAmplitudes[I];
	}

	NormalizeStateVector();
	UE_LOG(LogTemp, Log, TEXT("Applied unitary transformation. New amplitudes calculated."));
}

void UQuantumSuperpositionComponent::Collapse()
{
	if (bIsCollapsed || States.Num() == 0)
	{
		return;
	}

	double Cumulative = 0.0;
	const double Rand = FMath::FRand();

	for (const FQuantumState& State : States)
	{
		const double Prob = State.Amplitude.GetMagnitude() * State.Amplitude.GetMagnitude();
		Cumulative += Prob;

		if (Rand <= Cumulative)
		{
			CollapsedState = State;
			bIsCollapsed = true;
			UE_LOG(LogTemp, Log, TEXT("Collapsed to state: %s"), *CollapsedState.StateName);
			break;
		}
	}
}
