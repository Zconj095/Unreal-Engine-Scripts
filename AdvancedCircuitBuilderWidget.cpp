#include "AdvancedCircuitBuilderWidget.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogAdvancedCircuitBuilder);

void UAdvancedCircuitBuilderWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (AddQubitButton)
    {
        AddQubitButton->OnClicked.AddDynamic(this, &UAdvancedCircuitBuilderWidget::AddQubit);
    }

    if (AddGateButton)
    {
        AddGateButton->OnClicked.AddDynamic(this, &UAdvancedCircuitBuilderWidget::AddGate);
    }

    if (UndoButton)
    {
        UndoButton->OnClicked.AddDynamic(this, &UAdvancedCircuitBuilderWidget::Undo);
    }

    if (RedoButton)
    {
        RedoButton->OnClicked.AddDynamic(this, &UAdvancedCircuitBuilderWidget::Redo);
    }
}

void UAdvancedCircuitBuilderWidget::AddQubit()
{
    UWorld* World = GetWorld();
    if (!World || !QubitActorClass)
    {
        UE_LOG(LogAdvancedCircuitBuilder, Warning, TEXT("Missing world or qubit actor class."));
        return;
    }

    FVector SpawnLocation = FVector(QubitActors.Num() * 200.f, 0.f, 0.f);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(*FString::Printf(TEXT("Qubit_%d"), QubitActors.Num()));

    AActor* QubitActor = World->SpawnActor<AActor>(QubitActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (QubitActor)
    {
        QubitActors.Add(QubitActor);
        UE_LOG(LogAdvancedCircuitBuilder, Log, TEXT("Added qubit %d"), QubitActors.Num() - 1);
    }
}

void UAdvancedCircuitBuilderWidget::AddGate()
{
    if (QubitActors.Num() == 0)
    {
        UE_LOG(LogAdvancedCircuitBuilder, Warning, TEXT("No qubits available to add gates."));
        return;
    }

    if (!GateDropdown)
    {
        UE_LOG(LogAdvancedCircuitBuilder, Warning, TEXT("Gate dropdown widget not bound."));
        return;
    }

    const FString SelectedGate = GateDropdown->GetSelectedOption();
    bool bGateAdded = false;
    FQuantumGate GateToAdd;

    if (SelectedGate.Equals(TEXT("Hadamard"), ESearchCase::IgnoreCase))
    {
        GateToAdd = FQuantumGate::CreateHadamard(0);
        bGateAdded = true;
    }
    else if (SelectedGate.Equals(TEXT("CNOT"), ESearchCase::IgnoreCase) && QubitActors.Num() > 1)
    {
        GateToAdd = FQuantumGate::CreateCNOT(0, 1);
        bGateAdded = true;
    }

    if (bGateAdded)
    {
        SaveStateToUndoStack();
        CircuitGates.Add(GateToAdd);
        UE_LOG(LogAdvancedCircuitBuilder, Log, TEXT("Added %s gate to the circuit."), *SelectedGate);

        if (GateActorClass)
        {
            UWorld* World = GetWorld();
            if (World)
            {
                FVector GateLocation = FVector(CircuitGates.Num() * 150.f, 0.f, 50.f);
                World->SpawnActor<AActor>(GateActorClass, GateLocation, FRotator::ZeroRotator);
            }
        }
    }
}

void UAdvancedCircuitBuilderWidget::Undo()
{
    if (UndoStack.Num() > 0)
    {
        RedoStack.Add(CircuitGates);
        CircuitGates = UndoStack.Last();
        UndoStack.Pop();
        UE_LOG(LogAdvancedCircuitBuilder, Log, TEXT("Undo action performed."));
    }
}

void UAdvancedCircuitBuilderWidget::Redo()
{
    if (RedoStack.Num() > 0)
    {
        UndoStack.Add(CircuitGates);
        CircuitGates = RedoStack.Last();
        RedoStack.Pop();
        UE_LOG(LogAdvancedCircuitBuilder, Log, TEXT("Redo action performed."));
    }
}

void UAdvancedCircuitBuilderWidget::SaveStateToUndoStack()
{
    UndoStack.Add(CircuitGates);
    RedoStack.Empty();
}
