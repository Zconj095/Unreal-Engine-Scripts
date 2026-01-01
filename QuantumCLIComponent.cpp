#include "QuantumCLIComponent.h"
#include "QuantumStorageSystemComponent.h"
#include "QuantumBoltzmannMachineComponent.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"

namespace
{
	void ThreadProc(TFunction<bool()> MainLoop)
	{
		while (MainLoop())
		{
			FPlatformProcess::Sleep(0.1f);
		}
	}
}

UQuantumCLIComponent::UQuantumCLIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumCLIComponent::BeginPlay()
{
	Super::BeginPlay();

	WaitForDependencies();
	bIsRunning = true;
	CLIThread = Async(EAsyncExecution::Thread, [this]()
	{
		CommandLoop();
	});
}

void UQuantumCLIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bIsRunning = false;
	CLIThread.Wait();

	Super::EndPlay(EndPlayReason);
}

void UQuantumCLIComponent::WaitForDependencies()
{
	QuantumStorage = Cast<UQuantumStorageSystemComponent>(GetOwner()->GetComponentByClass(UQuantumStorageSystemComponent::StaticClass()));
	BoltzmannMachine = Cast<UQuantumBoltzmannMachineComponent>(GetOwner()->GetComponentByClass(UQuantumBoltzmannMachineComponent::StaticClass()));

	double Timeout = 0.0;
	while ((!QuantumStorage || !BoltzmannMachine) && Timeout < 10.0)
	{
		FPlatformProcess::Sleep(1.0f);
		Timeout += 1.0;
		QuantumStorage = Cast<UQuantumStorageSystemComponent>(GetOwner()->GetComponentByClass(UQuantumStorageSystemComponent::StaticClass()));
		BoltzmannMachine = Cast<UQuantumBoltzmannMachineComponent>(GetOwner()->GetComponentByClass(UQuantumBoltzmannMachineComponent::StaticClass()));
	}
}

void UQuantumCLIComponent::CommandLoop()
{
	while (bIsRunning)
	{
		// Placeholder: integrate actual console input here if desired.
		FPlatformProcess::Sleep(1.0f);
	}
}

void UQuantumCLIComponent::ProcessCommand(const FString& Command)
{
	if (!QuantumStorage || !BoltzmannMachine)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumCLI: Dependencies not ready."));
		return;
	}

	TArray<FString> Args;
	Command.ParseIntoArrayWS(Args);
	if (Args.Num() == 0)
	{
		return;
	}

	const FString Cmd = Args[0].ToLower();
	if (Cmd == TEXT("status"))
	{
		DisplayStatus();
	}
	else if (Cmd == TEXT("list"))
	{
		ListStoredFiles();
	}
	else if (Cmd == TEXT("train"))
	{
		TrainAI();
	}
	else if (Cmd == TEXT("analyze") && Args.Num() > 1)
	{
		AnalyzeFile(Args[1]);
	}
}

void UQuantumCLIComponent::ExecuteCommand(const FString& Command)
{
	ProcessCommand(Command);
}

void UQuantumCLIComponent::DisplayStatus() const
{
	UE_LOG(LogTemp, Log, TEXT("Quantum AI System Status:"));
	UE_LOG(LogTemp, Log, TEXT("Quantum Storage: %s"), QuantumStorage && QuantumStorage->IsMounted() ? TEXT("Mounted") : TEXT("Not Mounted"));
	if (BoltzmannMachine)
	{
		UE_LOG(LogTemp, Log, TEXT("Trained Files: %d"), BoltzmannMachine->GetProcessedFileCount());
	}
}

void UQuantumCLIComponent::ListStoredFiles() const
{
	if (!QuantumStorage)
	{
		return;
	}

	TArray<FString> Files = QuantumStorage->GetStoredFiles();
	for (const FString& File : Files)
	{
		UE_LOG(LogTemp, Log, TEXT("- %s"), *File);
	}
}

void UQuantumCLIComponent::TrainAI() const
{
	if (BoltzmannMachine)
	{
		BoltzmannMachine->TrainBoltzmannMachine();
	}
}

void UQuantumCLIComponent::AnalyzeFile(const FString& Filename) const
{
	if (!QuantumStorage || !BoltzmannMachine)
	{
		return;
	}

	if (QuantumStorage->FileExists(Filename))
	{
		BoltzmannMachine->AnalyzeTextFile(Filename);
	}
}
