// Copyright Epic Games, Inc.

#include "QuantumFieldTranslucencyComponent.h"

void UQuantumFieldTranslucencyComponent::CalculateTranslucency(float FluxOutput, int32 ExpectedSize)
{
	if (FluxOutput != LastFluxOutput)
	{
		if (ExpectedSize <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Expected size for translucency data must be greater than zero."));
			return;
		}

		TranslucencyData.SetNum(ExpectedSize);
		for (int32 i = 0; i < ExpectedSize; ++i)
		{
			TranslucencyData[i] = FMath::Abs(FMath::Sin(FluxOutput + i * 0.1f));
		}

		LastFluxOutput = FluxOutput;
		UE_LOG(LogTemp, Log, TEXT("Translucency data calculated with fluxOutput: %f and size: %d"), FluxOutput, ExpectedSize);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Flux output unchanged. Using cached translucency data."));
	}
}

const TArray<float>& UQuantumFieldTranslucencyComponent::GetTranslucencyData() const
{
	if (TranslucencyData.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Translucency data is not initialized. Returning empty array."));
	}
	return TranslucencyData;
}
