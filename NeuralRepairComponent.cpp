#include "NeuralRepairComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

UNeuralRepairComponent::UNeuralRepairComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralRepairComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeFields();
	RepairPower = CalculateRepairPower();
	UE_LOG(LogTemp, Log, TEXT("Repair Power (P_r): %0.4f"), RepairPower);
}

void UNeuralRepairComponent::TickComponent(float DeltaTimeSeconds, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTimeSeconds, TickType, ThisTickFunction);

	if (bDrawDebugGizmos && MagicalEnergyDensity.Num() == TimeSteps && EnergyFlowGradient.Num() == TimeSteps)
	{
		DrawDebugFields();
	}
}

void UNeuralRepairComponent::InitializeFields()
{
	MagicalEnergyDensity.SetNum(TimeSteps);
	EnergyFlowGradient.SetNum(TimeSteps);

	FRandomStream RandomStream(FDateTime::Now().GetTicks());
	for (int32 Index = 0; Index < TimeSteps; ++Index)
	{
		MagicalEnergyDensity[Index] = RandomStream.GetFraction();
		EnergyFlowGradient[Index] = RandomStream.GetFraction();
	}

	UE_LOG(LogTemp, Log, TEXT("Fields initialized."));
}

double UNeuralRepairComponent::CalculateRepairPower() const
{
	if (TimeSteps <= 0 || MagicalEnergyDensity.Num() == 0 || EnergyFlowGradient.Num() == 0)
	{
		return 0.0;
	}

	const double TotalTime = TimeSteps * DeltaTime;
	const double Dx = DeltaTime;
	double Sum = 0.0;

	for (int32 Index = 0; Index < TimeSteps; ++Index)
	{
		const double Value = MagicalEnergyDensity[Index] * EnergyFlowGradient[Index];
		Sum += Value * Dx;
	}

	return Sum;
}

void UNeuralRepairComponent::DrawDebugFields() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (int32 Index = 0; Index < TimeSteps; ++Index)
	{
		const float X = Index * DeltaTime;
		const float Y = static_cast<float>(MagicalEnergyDensity.IsValidIndex(Index) ? MagicalEnergyDensity[Index] : 0.0);
		const float Z = static_cast<float>(EnergyFlowGradient.IsValidIndex(Index) ? EnergyFlowGradient[Index] : 0.0);
		const FVector Location = (GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector) + FVector(X, Y, Z);
		const FLinearColor Color = FLinearColor(Y, Z, 1.0f);
		DrawDebugSphere(World, Location, 0.1f, 8, Color.ToFColor(true), false, DeltaTime);
	}
}
