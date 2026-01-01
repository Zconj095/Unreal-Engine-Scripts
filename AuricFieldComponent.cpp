#include "AuricFieldComponent.h"

#include "Engine/Engine.h"

UAuricFieldComponent::UAuricFieldComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Initialize to match Unity dictionary literals
    AuricFrequencies.Add(TEXT("Physical"), 0.01f);
    AuricFrequencies.Add(TEXT("Emotional"), 0.1f);
    AuricFrequencies.Add(TEXT("Mental"), 0.4f);
    AuricFrequencies.Add(TEXT("Spiritual"), 2.0f);

    EndocrineMapping.Add(TEXT("Root"), TEXT("Adrenal"));
    EndocrineMapping.Add(TEXT("Sacral"), TEXT("Gonads"));
    EndocrineMapping.Add(TEXT("Solar Plexus"), TEXT("Pancreas"));
    EndocrineMapping.Add(TEXT("Heart"), TEXT("Thymus"));
    EndocrineMapping.Add(TEXT("Throat"), TEXT("Thyroid"));
    EndocrineMapping.Add(TEXT("Third Eye"), TEXT("Pituitary"));
    EndocrineMapping.Add(TEXT("Crown"), TEXT("Pineal"));

    ChakraFrequencies.Add(TEXT("Root"), 0.1f);
    ChakraFrequencies.Add(TEXT("Sacral"), 0.2f);
    ChakraFrequencies.Add(TEXT("Solar Plexus"), 0.3f);
    ChakraFrequencies.Add(TEXT("Heart"), 0.5f);
    ChakraFrequencies.Add(TEXT("Throat"), 0.7f);
    ChakraFrequencies.Add(TEXT("Third Eye"), 0.9f);
    ChakraFrequencies.Add(TEXT("Crown"), 1.0f);
}

void UAuricFieldComponent::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Auric Frequencies and Chakras Initialized."));
}

