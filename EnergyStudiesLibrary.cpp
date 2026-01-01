#include "EnergyStudiesLibrary.h"

FString UEnergyStudiesLibrary::ToLabel_Acquisition(EEnergyAcquisitionType V)
{
    switch (V)
    {
    case EEnergyAcquisitionType::Borrowed:   return TEXT("Borrowed Energy");
    case EEnergyAcquisitionType::Gathered:   return TEXT("Gathered Energy");
    case EEnergyAcquisitionType::Earned:     return TEXT("Earned Energy");
    case EEnergyAcquisitionType::Obtained:   return TEXT("Obtained Energy");
    case EEnergyAcquisitionType::Economical: return TEXT("Economical Energy");
    case EEnergyAcquisitionType::Stored:     return TEXT("Stored Energy");
    default:                                 return TEXT("Required Energy");
    }
}

FString UEnergyStudiesLibrary::DefinitionOf_EnergyClass(EEnergyClass Class)
{
    switch (Class)
    {
    case EEnergyClass::NaturalEnergy:
        return TEXT("Natural energies-The natural form of energies itself without any form of artificial connections");
    case EEnergyClass::ArtificialEnergy:
        return TEXT("Artificial energies-The creation of energies using artificial techniques that are natural");
    }
    return FString();
}

FString UEnergyStudiesLibrary::DefinitionOf_EnergyType(EEnergyType Type)
{
    switch (Type)
    {
    case EEnergyType::Willed:
        return TEXT("Willed energy-A class of energy that is used by the sole consent of the source connection for the energy to be applied and required for both host and source to be in agreement for energy to be applied");
    case EEnergyType::Pressured:
        return TEXT("Pressured energy-A class of energy influenced by pressure applied to the source connection to elicit effects");
    case EEnergyType::Forced:
        return TEXT("Forced energy-A class of energy that is used by applying pressure or force to the source connection for effects to happen");
    }
    return FString();
}

FString UEnergyStudiesLibrary::DefinitionOf_Acquisition(EEnergyAcquisitionType Type)
{
    switch (Type)
    {
    case EEnergyAcquisitionType::Borrowed:
        return TEXT("Borrowed Energy – A type of energy that is obtained by gathering or taking from another source other than the original host");
    case EEnergyAcquisitionType::Gathered:
        return TEXT("Gathered Energy – A type of energy gained by gathering energy inwards toward the host");
    case EEnergyAcquisitionType::Earned:
        return TEXT("Earned Energy – A type of energy obtained by doing work or by forming a success from a specific amount of effort");
    case EEnergyAcquisitionType::Obtained:
        return TEXT("Obtained Energy – A type of energy gained by a host taking from a specific source or location");
    case EEnergyAcquisitionType::Economical:
        return TEXT("Economical Energy – A type of energy gained by living within an environment");
    case EEnergyAcquisitionType::Stored:
        return TEXT("Stored Energy – A type of energy that exists as stored reservoirs of not used energy from a previous point within time or space");
    case EEnergyAcquisitionType::Required:
        return TEXT("Required Energy – A type of energy that is required to exist");
    }
    return FString();
}

TArray<EEnergyAcquisitionType> UEnergyStudiesLibrary::AllAcquisitionTypes()
{
    return {
        EEnergyAcquisitionType::Borrowed,
        EEnergyAcquisitionType::Gathered,
        EEnergyAcquisitionType::Earned,
        EEnergyAcquisitionType::Obtained,
        EEnergyAcquisitionType::Economical,
        EEnergyAcquisitionType::Stored,
        EEnergyAcquisitionType::Required
    };
}

