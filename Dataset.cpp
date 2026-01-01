#include "Dataset.h"

FDataset::FDataset() = default;

FDataset::FDataset(const TArray<FDataPoint>& InDataPoints)
	: DataPoints(InDataPoints)
{
}
