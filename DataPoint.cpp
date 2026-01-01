#include "DataPoint.h"

FDataPoint::FDataPoint() = default;

FDataPoint::FDataPoint(const TArray<float>& InFeatures, int32 InLabel)
	: Features(InFeatures)
	, Label(InLabel)
{
}
