#include "BatchProcessorLibrary.h"

void UBatchProcessorLibrary::ProcessBatch(const TArray<FFloatArray>& Data, int32 BatchSize, TArray<FFloatArrayBatch>& OutBatches)
{
    OutBatches.Reset();
    if (BatchSize <= 0 || Data.Num() == 0)
    {
        return;
    }

    for (int32 i = 0; i < Data.Num(); i += BatchSize)
    {
        FFloatArrayBatch Batch;
        const int32 Count = FMath::Min(BatchSize, Data.Num() - i);
        Batch.Items.Reserve(Count);
        for (int32 k = 0; k < Count; ++k)
        {
            Batch.Items.Add(Data[i + k]);
        }
        OutBatches.Add(MoveTemp(Batch));
    }
}

