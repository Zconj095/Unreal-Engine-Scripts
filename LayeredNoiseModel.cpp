// Copyright Epic Games, Inc.

#include "LayeredNoiseModel.h"

#include "ComplexNumber.h"

void ULayeredNoiseModel::AddNoiseLayer(TScriptInterface<IQuantumNoiseModel> Noise)
{
	if (Noise)
	{
		NoiseLayers.Add(Noise);
	}
}

void ULayeredNoiseModel::ApplyNoise(UPARAM(ref) TArray<FComplexNumber>& StateVector)
{
	for (const TScriptInterface<IQuantumNoiseModel>& Noise : NoiseLayers)
	{
		if (Noise)
		{
			Noise->Execute_ApplyNoise(Noise.GetObject(), StateVector);
		}
	}
}
