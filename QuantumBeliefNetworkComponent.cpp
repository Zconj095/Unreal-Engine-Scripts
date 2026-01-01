#include "QuantumBeliefNetworkComponent.h"

#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"

UQuantumBeliefNetworkComponent::UQuantumBeliefNetworkComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumBeliefNetworkComponent::BeginPlay()
{
    Super::BeginPlay();
    LoadPlugin();
}

void UQuantumBeliefNetworkComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (PluginHandle)
    {
        FPlatformProcess::FreeDllHandle(PluginHandle);
        PluginHandle = nullptr;
        ForwardFunction = nullptr;
    }
    Super::EndPlay(EndPlayReason);
}

void UQuantumBeliefNetworkComponent::LoadPlugin()
{
    if (PluginHandle)
    {
        return;
    }

    const FString PluginPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/ThirdParty/QATF_Plugin"));
#if PLATFORM_WINDOWS
    const FString LibPath = FPaths::Combine(*PluginPath, TEXT("QATF_Plugin.dll"));
#else
    const FString LibPath = FPaths::Combine(*PluginPath, TEXT("libQATF_Plugin.so"));
#endif

    PluginHandle = FPlatformProcess::GetDllHandle(*LibPath);
    if (PluginHandle)
    {
        ForwardFunction = reinterpret_cast<FForwardPropagationFunc>(FPlatformProcess::GetDllExport(PluginHandle, TEXT("ForwardPropagation")));
    }
}

bool UQuantumBeliefNetworkComponent::RunForwardPropagation(const TArray<float>& Inputs, TArray<float>& Outputs)
{
    if (!ForwardFunction || Inputs.Num() == 0)
    {
        return false;
    }

    Outputs.SetNumZeroed(Outputs.Num());
    ForwardFunction(Inputs.GetData(), Inputs.Num(), Outputs.GetData(), Outputs.Num());
    return true;
}
