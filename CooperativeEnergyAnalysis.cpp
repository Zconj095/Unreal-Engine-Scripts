#include "CooperativeEnergyAnalysis.h"
#include "TensorFlowWrapper.h"

DEFINE_LOG_CATEGORY(LogCooperativeEnergyAnalysis);

UCooperativeEnergyAnalysis::UCooperativeEnergyAnalysis()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCooperativeEnergyAnalysis::BeginPlay()
{
    Super::BeginPlay();

    EnergySynergy = CalculateEnergySynergy();
    UE_LOG(LogCooperativeEnergyAnalysis, Log, TEXT("Synergistic Energy Efficiency (E_synergy): %f"), EnergySynergy);

    TensorFlowValidation();
}

double UCooperativeEnergyAnalysis::CalculateEnergySynergy() const
{
    if (FMath::IsNearlyZero(TotalEnergyInput))
    {
        UE_LOG(LogCooperativeEnergyAnalysis, Warning, TEXT("Total energy input must be non-zero."));
        return 0.0;
    }
    return static_cast<double>(BiologicalEnergy + MagicalEnergy) / static_cast<double>(TotalEnergyInput);
}

void UCooperativeEnergyAnalysis::TensorFlowValidation() const
{
    auto Graph = MakeShared<FTFGraph>();
    const auto TfEbio = Graph->Const(BiologicalEnergy);
    const auto TfEmag = Graph->Const(MagicalEnergy);
    const auto TfEinput = Graph->Const(TotalEnergyInput);
    const auto TfEnergySynergy = Graph->Div(Graph->Add(TfEbio, TfEmag), TfEinput);

    FTFSession Session(Graph);
    const auto Result = Session.RunSingle(TfEnergySynergy);
    UE_LOG(LogCooperativeEnergyAnalysis, Log, TEXT("TensorFlow Validation - Synergistic Energy Efficiency (E_synergy): %f"), Result);
}
