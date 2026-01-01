#include "LLMManagerComponent.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"
// JSON payload support for advanced prompting
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"

ULLMManagerComponent::ULLMManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULLMManagerComponent::BeginPlay()
{
    Super::BeginPlay();
    if (bAutoConnect)
    {
        ConnectToBackend();
    }
}

void ULLMManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Disconnect();
    Super::EndPlay(EndPlayReason);
}

bool ULLMManagerComponent::ConnectToBackend()
{
    if (Socket)
    {
        return true;
    }

    ISocketSubsystem* Sockets = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!Sockets)
    {
        return false;
    }

    FIPv4Address IP;
    if (!FIPv4Address::Parse(Host, IP))
    {
        // Fallback to localhost
        IP = FIPv4Address(127,0,0,1);
    }

    TSharedRef<FInternetAddr> Addr = Sockets->CreateInternetAddr();
    Addr->SetIp(IP.Value);
    Addr->SetPort(Port);

    Socket = Sockets->CreateSocket(NAME_Stream, TEXT("LLMClient"), false);
    if (!Socket)
    {
        return false;
    }

    Socket->SetReuseAddr(true);
    Socket->SetNonBlocking(false); // blocking to mimic synchronous Unity behavior

    const bool bConnected = Socket->Connect(*Addr);
    if (!bConnected)
    {
        Sockets->DestroySocket(Socket);
        Socket = nullptr;
        return false;
    }
    return true;
}

bool ULLMManagerComponent::IsConnected() const
{
    return Socket != nullptr;
}

bool ULLMManagerComponent::SendPrompt(const FString& Prompt, FString& OutResponse)
{
    OutResponse.Reset();
    if (!Socket)
    {
        return false;
    }

    // Send UTF-8 bytes
    FTCHARToUTF8 Utf8(*Prompt);
    int32 BytesSent = 0;
    if (!Socket->Send(reinterpret_cast<const uint8*>(Utf8.Get()), Utf8.Length(), BytesSent))
    {
        return false;
    }

    // Read up to 1024 bytes (single read like the Unity example)
    uint8 Buffer[1024];
    int32 BytesRead = 0;
    if (!Socket->Recv(Buffer, sizeof(Buffer), BytesRead))
    {
        return false;
    }

    FUTF8ToTCHAR Back(reinterpret_cast<const ANSICHAR*>(Buffer), BytesRead);
    OutResponse = FString(Back.Get(), Back.Length());
    return true;
}

bool ULLMManagerComponent::SendPromptWithTemperature(const FString& Prompt, FString& OutResponse)
{
    OutResponse.Reset();
    if (!Socket)
    {
        return false;
    }

    // Build JSON: { "prompt": "...", "temperature": <float> }
    TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
    Root->SetStringField(TEXT("prompt"), Prompt);
    Root->SetNumberField(TEXT("temperature"), static_cast<double>(Temperature));
    Root->SetNumberField(TEXT("batch_size"), static_cast<double>(BatchSize));
    Root->SetNumberField(TEXT("context_size"), static_cast<double>(ContextSize));
    Root->SetNumberField(TEXT("gpu_layers"), static_cast<double>(GPULayers));

    FString JsonStr;
    {
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
        if (!FJsonSerializer::Serialize(Root, Writer))
        {
            return false;
        }
    }

    // Send UTF-8 JSON bytes
    FTCHARToUTF8 Utf8(*JsonStr);
    int32 BytesSent = 0;
    if (!Socket->Send(reinterpret_cast<const uint8*>(Utf8.Get()), Utf8.Length(), BytesSent))
    {
        return false;
    }

    // Read up to 1024 bytes of reply
    uint8 Buffer[1024];
    int32 BytesRead = 0;
    if (!Socket->Recv(Buffer, sizeof(Buffer), BytesRead))
    {
        return false;
    }

    FUTF8ToTCHAR Back(reinterpret_cast<const ANSICHAR*>(Buffer), BytesRead);
    OutResponse = FString(Back.Get(), Back.Length());
    return true;
}

void ULLMManagerComponent::Disconnect()
{
    if (Socket)
    {
        ISocketSubsystem* Sockets = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
        Socket->Close();
        if (Sockets)
        {
            Sockets->DestroySocket(Socket);
        }
        Socket = nullptr;
    }
}
