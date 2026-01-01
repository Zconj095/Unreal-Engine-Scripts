#include "ElectronConfigurationComponent.h"

UElectronConfigurationComponent::UElectronConfigurationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Default filling order matching the Unity example
	FillingOrder = {
		TEXT("1s"), TEXT("2s"), TEXT("2p"), TEXT("3s"), TEXT("3p"), TEXT("4s"), TEXT("3d"), TEXT("4p"),
		TEXT("5s"), TEXT("4d"), TEXT("5p"), TEXT("6s"), TEXT("4f"), TEXT("5d"), TEXT("6p"), TEXT("7s"), TEXT("5f"), TEXT("6d"), TEXT("7p")
	};
}

void UElectronConfigurationComponent::BeginPlay()
{
	Super::BeginPlay();
	if (bAutoGenerateOnBeginPlay)
	{
		GenerateElectronConfiguration(AtomicNumber);
	}
}

void UElectronConfigurationComponent::GenerateElectronConfiguration(int32 InAtomicNumber)
{
	AtomicNumber = FMath::Max(0, InAtomicNumber);
	ElectronConfiguration.Reset();

	int32 Remaining = AtomicNumber;
	for (const FString& Subshell : FillingOrder)
	{
		if (Remaining <= 0)
		{
			break;
		}

		// Parse energy level (one or more digits at start)
		int32 i = 0;
		while (i < Subshell.Len() && FChar::IsDigit(Subshell[i]))
		{
			++i;
		}
		const FString LevelStr = Subshell.Left(i);
		const int32 EnergyLevel = FCString::Atoi(*LevelStr);
		const TCHAR Orbital = (i < Subshell.Len()) ? Subshell[i] : TEXT('\0');
		const int32 MaxInSubshell = GetMaxElectronsInSubshell(Orbital);
		if (MaxInSubshell <= 0)
		{
			continue;
		}

		const int32 ElectronsHere = FMath::Min(Remaining, MaxInSubshell);
		FElectronSubshell Entry;
		Entry.EnergyLevel = EnergyLevel;
		Entry.OrbitalType = FName(Orbital == 0 ? TEXT("") : FString::Chr(Orbital));
		Entry.ElectronCount = ElectronsHere;
		ElectronConfiguration.Add(Entry);

		Remaining -= ElectronsHere;
	}
}

int32 UElectronConfigurationComponent::GetMaxElectronsInSubshell(TCHAR OrbitalType) const
{
	switch (OrbitalType)
	{
	case TEXT('s'): return 2;  // s can hold 2
	case TEXT('p'): return 6;  // p can hold 6
	case TEXT('d'): return 10; // d can hold 10
	case TEXT('f'): return 14; // f can hold 14
	default: return 0;
	}
}

