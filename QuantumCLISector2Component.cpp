#include "QuantumCLISector2Component.h"
#include "QuantumBoltzmannMachineComponent.h"
#include "QuantumFluxGaugeComponent.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"

UQuantumCLISector2Component::UQuantumCLISector2Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumCLISector2Component::BeginPlay()
{
	Super::BeginPlay();

	BoltzmannMachine = Cast<UQuantumBoltzmannMachineComponent>(GetOwner()->GetComponentByClass(UQuantumBoltzmannMachineComponent::StaticClass()));
	FluxGauge = Cast<UQuantumFluxGaugeComponent>(GetOwner()->GetComponentByClass(UQuantumFluxGaugeComponent::StaticClass()));

	bIsRunning = true;
	CLIThread = Async(EAsyncExecution::Thread, [this]()
	{
		CommandLoop();
	});
}

void UQuantumCLISector2Component::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bIsRunning = false;
	CLIThread.Wait();
	Super::EndPlay(EndPlayReason);
}

namespace
{
	FString ReadConsoleLine()
	{
		return TEXT("");
	}
}

void UQuantumCLISector2Component::CommandLoop()
{
	while (bIsRunning)
	{
		FPlatformProcess::Sleep(1.f);
	}
}

void UQuantumCLISector2Component::ProcessCommand(const FString& Command)
{
	if (!BoltzmannMachine)
	{
		return;
	}

	TArray<FString> Args;
	Command.ParseIntoArrayWS(Args);
	if (Args.Num() == 0)
	{
		return;
	}

	const FString Cmd = Args[0].ToLower();
	if (Cmd == TEXT("hebbian"))
	{
		AnalyzeHebbianClusters();
	}
	else if (Cmd == TEXT("flux"))
	{
		AnalyzeFluxGauge();
	}
}

void UQuantumCLISector2Component::AnalyzeHebbianClusters() const
{
	if (!BoltzmannMachine)
	{
		return;
	}

	const TMap<int32, TArray<float>> Clusters = BoltzmannMachine->GetHebbianClusters();
	for (const auto& Cluster : Clusters)
	{
		UE_LOG(LogTemp, Log, TEXT("Cluster %d: %d weights"), Cluster.Key, Cluster.Value.Num());
	}
}

void UQuantumCLISector2Component::AnalyzeFluxGauge() const
{
	if (FluxGauge)
	{
		FluxGauge->AnalyzeSuperimposedStates();
		FluxGauge->DisplayFluxData();
	}
}
