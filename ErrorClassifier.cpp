#include "ErrorClassifier.h"

FString UErrorClassifier::ClassifyError(const TArray<float>& Errors)
{
	if (Errors.Num() == 0)
	{
		return TEXT("Undefined");
	}

	float Sum = 0.0f;
	for (float Value : Errors)
	{
		Sum += Value;
	}
	const float MeanError = Sum / Errors.Num();

	if (MeanError < 0.1f)
	{
		return TEXT("Low");
	}
	if (MeanError < 0.5f)
	{
		return TEXT("Medium");
	}

	return TEXT("High");
}
