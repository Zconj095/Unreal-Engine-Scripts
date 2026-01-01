#include "QuantumReconstructorComponent.h"
#include "QuantumHypervectorComponent.h"
#include "QuantumBoltzmannMachineComponent.h"
#include "NeuralLayer.h"
#include "Async/Async.h"
#include "Engine/World.h"

UQuantumReconstructorComponent::UQuantumReconstructorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumReconstructorComponent::BeginPlay()
{
	Super::BeginPlay();

	Hypervector = Cast<UQuantumHypervectorComponent>(GetOwner()->GetComponentByClass(UQuantumHypervectorComponent::StaticClass()));
	Boltzmann = Cast<UQuantumBoltzmannMachineComponent>(GetOwner()->GetComponentByClass(UQuantumBoltzmannMachineComponent::StaticClass()));

	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		ReconstructBoltzmannMachine();
	});
}

bool UQuantumReconstructorComponent::WaitForHypervector(float TimeoutSeconds)
{
	if (!Hypervector)
	{
		return false;
	}

	const double StartTime = FPlatformTime::Seconds();
	while (!Hypervector->IsReady() && (FPlatformTime::Seconds() - StartTime) < TimeoutSeconds)
	{
		FPlatformProcess::Sleep(0.5f);
	}
	return Hypervector->IsReady();
}

void UQuantumReconstructorComponent::ReconstructBoltzmannMachine()
{
	if (!Boltzmann || !Hypervector)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumReconstructor: missing dependencies."));
		return;
	}

	if (!WaitForHypervector())
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumReconstructor: Hypervector failed to become ready."));
		return;
	}

	TArray<UNeuralLayer*> ReconstructedLayers;
	for (int32 LayerIndex = 0; LayerIndex < 3; ++LayerIndex)
	{
		TArray<float> HypervectorData = Hypervector->GetHypervector(LayerIndex);
		if (HypervectorData.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("QuantumReconstructor: No hypervector found for layer %d"), LayerIndex);
			continue;
		}

		UNeuralLayer* NewLayer = NewObject<UNeuralLayer>(this);
		NewLayer->Initialize(LayerIndex, HypervectorData.Num());
		NewLayer->SetWeights(HypervectorData);
		ReconstructedLayers.Add(NewLayer);
	}

	Boltzmann->OverrideNeuralLayers(ReconstructedLayers);
	UE_LOG(LogTemp, Log, TEXT("QuantumReconstructor: Boltzmann machine reconstructed from hypervectors."));
}
