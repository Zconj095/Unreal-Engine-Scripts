#include "QuantumExternalCLIComponent.h"
#include "QuantumCLIComponent.h"
#include "Misc/Paths.h"
#include "Misc/ScopeLock.h"
#include "HAL/PlatformProcess.h"
#include "Async/Async.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

UQuantumExternalCLIComponent::UQuantumExternalCLIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumExternalCLIComponent::BeginPlay()
{
	Super::BeginPlay();

	QuantumCLI = Cast<UQuantumCLIComponent>(GetOwner()->GetComponentByClass(UQuantumCLIComponent::StaticClass()));

	bIsRunning = true;
	ServerFuture = Async(EAsyncExecution::ThreadPool, [this]() { PipeServerLoop(); });

	LaunchExternalCLI();
}

void UQuantumExternalCLIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bIsRunning = false;
	if (ServerFuture.IsValid())
	{
		ServerFuture.Wait();
	}

	Super::EndPlay(EndPlayReason);
}

void UQuantumExternalCLIComponent::LaunchExternalCLI()
{
	const FString ExePath = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("External/QuantumCLIExternal.exe"));
	if (!FPaths::FileExists(ExePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumExternalCLI: External CLI executable missing (%s)."), *ExePath);
		return;
	}

	FPlatformProcess::CreateProc(*ExePath, nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
}

void UQuantumExternalCLIComponent::ProcessReceivedCommand(const FString& Command)
{
	if (!QuantumCLI)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumExternalCLI: QuantumCLIComponent not found."));
		return;
	}

	QuantumCLI->ExecuteCommand(Command);
}

void UQuantumExternalCLIComponent::PipeServerLoop()
{
#if PLATFORM_WINDOWS
	const FString PipeName = TEXT("\\\\.\\pipe\\QuantumCLIPipe");
	const DWORD BufferSize = 2048;

	while (bIsRunning)
	{
		HANDLE Pipe = CreateNamedPipeW(*PipeName,
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			BufferSize,
			BufferSize,
			NMPWAIT_USE_DEFAULT_WAIT,
			nullptr);

		if (Pipe == INVALID_HANDLE_VALUE)
		{
			FPlatformProcess::Sleep(1.f);
			continue;
		}

		const bool bConnected = ConnectNamedPipe(Pipe, nullptr) ? true : (::GetLastError() == ERROR_PIPE_CONNECTED);
		if (!bConnected)
		{
			CloseHandle(Pipe);
			continue;
		}

		char Buffer[BufferSize];
		DWORD BytesRead = 0;

		while (bIsRunning && ReadFile(Pipe, Buffer, BufferSize - 1, &BytesRead, nullptr) && BytesRead > 0)
		{
			Buffer[BytesRead] = '\0';
			const FString Command = ANSI_TO_TCHAR(Buffer);

			AsyncTask(ENamedThreads::GameThread, [this, Command]()
			{
				ProcessReceivedCommand(Command);
			});
		}

		FlushFileBuffers(Pipe);
		DisconnectNamedPipe(Pipe);
		CloseHandle(Pipe);
	}
#else
	UE_LOG(LogTemp, Warning, TEXT("QuantumExternalCLI: Named pipes are only supported on Windows platforms."));
#endif
}
