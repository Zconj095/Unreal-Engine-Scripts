#include "MultiLocationalOriginBasisComponent.h"

#include "Logging/LogMacros.h"

UMultiLocationalOriginBasisComponent::UMultiLocationalOriginBasisComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMultiLocationalOriginBasisComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize example origins if none provided
	if (Origins.Num() == 0)
	{
		Origins.Add({TEXT("Origin1"), FVector::ZeroVector, FRotator::ZeroRotator});
		Origins.Add({TEXT("Origin2"), FVector(1000.f, 0.f, 0.f), FRotator(0.f, 45.f, 0.f)}); // 10m in UE units
		Origins.Add({TEXT("Origin3"), FVector(-1000.f, 0.f, 1000.f), FRotator(0.f, 90.f, 0.f)});
	}

	if (ObjectToTransform)
	{
		SetObjectRelativeToOrigin(CurrentOriginName);
	}
}

void UMultiLocationalOriginBasisComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ObjectToTransform)
	{
		SetObjectRelativeToOrigin(CurrentOriginName);
	}
}

TOptional<FOrigin> UMultiLocationalOriginBasisComponent::GetOriginByName(const FString& Name) const
{
	for (const FOrigin& Origin : Origins)
	{
		if (Origin.Name == Name)
		{
			return Origin;
		}
	}
	return TOptional<FOrigin>();
}

void UMultiLocationalOriginBasisComponent::SetObjectRelativeToOrigin(const FString& OriginName)
{
	const TOptional<FOrigin> Origin = GetOriginByName(OriginName);
	if (Origin.IsSet() && ObjectToTransform)
	{
		ObjectToTransform->SetActorLocation(Origin->Position);
		ObjectToTransform->SetActorRotation(Origin->Rotation);
		UE_LOG(LogTemp, Log, TEXT("Object set relative to %s: Position %s, Rotation %s"), *OriginName, *Origin->Position.ToString(), *Origin->Rotation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Origin not found: %s"), *OriginName);
	}
}

void UMultiLocationalOriginBasisComponent::AddOrigin(const FString& Name, const FVector& Position, const FRotator& Rotation)
{
	if (GetOriginByName(Name).IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Origin with name %s already exists."), *Name);
		return;
	}

	Origins.Add({Name, Position, Rotation});
	UE_LOG(LogTemp, Log, TEXT("New origin added: %s"), *Name);
}

void UMultiLocationalOriginBasisComponent::SwitchOrigin(const FString& NewOriginName)
{
	if (GetOriginByName(NewOriginName).IsSet())
	{
		CurrentOriginName = NewOriginName;
		UE_LOG(LogTemp, Log, TEXT("Switched to origin: %s"), *NewOriginName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Origin not found: %s"), *NewOriginName);
	}
}
