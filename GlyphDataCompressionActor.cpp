#include "GlyphDataCompressionActor.h"

#include "Containers/Array.h"
#include "Math/UnrealMathUtility.h"

AGlyphDataCompressionActor::AGlyphDataCompressionActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGlyphDataCompressionActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<double> Probabilities = NormalizeFrequencies(GlyphFrequencies);
	double Entropy = CalculateEntropy(Probabilities);

	if (bLogResult)
	{
		FString ProbString;
		for (double Value : Probabilities)
		{
			ProbString += FString::Printf(TEXT("%.3f "), Value);
		}
		UE_LOG(LogTemp, Display, TEXT("Glyph probabilities: %s"), *ProbString);
		UE_LOG(LogTemp, Display, TEXT("Entropy of Glyph Data: %.6f bits"), Entropy);
	}
}

double AGlyphDataCompressionActor::CalculateEntropy(const TArray<double>& Probabilities) const
{
	double Entropy = 0.0;
	for (double P : Probabilities)
	{
		if (P > 0.0)
		{
			Entropy += P * FMath::Loge(P) / FMath::Loge(2.0);
		}
	}
	return -Entropy;
}

TArray<double> AGlyphDataCompressionActor::NormalizeFrequencies(const TMap<FString, int32>& Frequencies) const
{
	int32 Total = 0;
	for (const TPair<FString, int32>& Pair : Frequencies)
	{
		Total += Pair.Value;
	}

	TArray<double> Result;
	Result.Reserve(Frequencies.Num());
	for (const TPair<FString, int32>& Pair : Frequencies)
	{
		if (Total > 0)
		{
			Result.Add(static_cast<double>(Pair.Value) / Total);
		}
		else
		{
			Result.Add(0.0);
		}
	}
	return Result;
}
