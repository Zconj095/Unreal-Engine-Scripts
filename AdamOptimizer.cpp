#include "AdamOptimizer.h"

#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY(LogAdamOptimizer);

FAdamOptimizer::FAdamOptimizer(
	int32 InMaxIterations /*= 10000*/,
	float InTolerance /*= 1.0e-6f*/,
	float InLearningRate /*= 1.0e-3f*/,
	float InBeta1 /*= 0.9f*/,
	float InBeta2 /*= 0.99f*/,
	float InNoiseFactor /*= 1.0e-8f*/,
	float InEpsilon /*= 1.0e-10f*/,
	bool bInUseAMSGrad /*= false*/,
	const FString& InSnapshotDirectory /*= TEXT("")*/)
	: MaxIterations(InMaxIterations)
	, Tolerance(InTolerance)
	, LearningRate(InLearningRate)
	, Beta1(InBeta1)
	, Beta2(InBeta2)
	, NoiseFactor(InNoiseFactor)
	, Epsilon(InEpsilon)
	, bUseAMSGrad(bInUseAMSGrad)
	, SnapshotDirectory(InSnapshotDirectory)
{
	Reset();
}

void FAdamOptimizer::Reset()
{
	TimeStep = 0;
	FirstMoment.Reset();
	SecondMoment.Reset();
	MaxSecondMoment.Reset();
}

FAdamOptimizationResult FAdamOptimizer::Minimize(
	TFunction<float(const TArray<float>&)> LossFunction,
	const TArray<float>& InitialParams,
	TFunction<TArray<float>(const TArray<float>&)> GradientFunction)
{
	const int32 ParamCount = InitialParams.Num();
	FirstMoment.Init(0.0f, ParamCount);
	SecondMoment.Init(0.0f, ParamCount);
	if (bUseAMSGrad)
	{
		MaxSecondMoment.Init(0.0f, ParamCount);
	}

	TArray<float> Parameters = InitialParams;
	TArray<float> Gradients = GradientFunction(Parameters);

	for (int32 Iteration = 0; Iteration < MaxIterations; ++Iteration)
	{
		++TimeStep;

		for (int32 Index = 0; Index < ParamCount; ++Index)
		{
			FirstMoment[Index] = Beta1 * FirstMoment[Index] + (1.0f - Beta1) * Gradients[Index];
			SecondMoment[Index] = Beta2 * SecondMoment[Index] + (1.0f - Beta2) * Gradients[Index] * Gradients[Index];

			if (bUseAMSGrad)
			{
				MaxSecondMoment[Index] = FMath::Max(MaxSecondMoment[Index], SecondMoment[Index]);
			}
		}

		const float CorrectedLearningRate = LearningRate * FMath::Sqrt(1.0f - FMath::Pow(Beta2, TimeStep)) /
			(1.0f - FMath::Pow(Beta1, TimeStep));

		for (int32 Index = 0; Index < ParamCount; ++Index)
		{
			const float Denominator = FMath::Sqrt(bUseAMSGrad ? MaxSecondMoment[Index] : SecondMoment[Index]) + NoiseFactor;
			Parameters[Index] -= CorrectedLearningRate * FirstMoment[Index] / Denominator;
		}

		if (!SnapshotDirectory.IsEmpty())
		{
			SaveSnapshot();
		}

		if (ComputeNorm(Gradients) < Tolerance)
		{
			break;
		}

		Gradients = GradientFunction(Parameters);
	}

	FAdamOptimizationResult Result;
	Result.Parameters = Parameters;
	Result.Loss = LossFunction(Parameters);
	return Result;
}

void FAdamOptimizer::LoadSnapshot(const FString& InSnapshotDirectory)
{
	if (InSnapshotDirectory.IsEmpty())
	{
		UE_LOG(LogAdamOptimizer, Warning, TEXT("Snapshot directory is empty."));
		return;
	}

	SnapshotDirectory = InSnapshotDirectory;
	const FString FilePath = FPaths::Combine(*SnapshotDirectory, TEXT("adam_params.csv"));

	TArray<FString> Lines;
	if (!FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		UE_LOG(LogAdamOptimizer, Warning, TEXT("Failed to load Adam snapshot from %s"), *FilePath);
		return;
	}

	const int32 LinesPerSnapshot = 3 + (bUseAMSGrad ? 1 : 0);
	if (Lines.Num() < LinesPerSnapshot)
	{
		UE_LOG(LogAdamOptimizer, Warning, TEXT("Snapshot file %s does not contain a complete snapshot."), *FilePath);
		return;
	}

	int32 StartLine = Lines.Num() - LinesPerSnapshot;
	FirstMoment = ParseVectorLine(Lines[StartLine++]);
	SecondMoment = ParseVectorLine(Lines[StartLine++]);
	if (bUseAMSGrad)
	{
		MaxSecondMoment = ParseVectorLine(Lines[StartLine++]);
	}
	TimeStep = FCString::Atoi(*Lines[StartLine]);
}

bool FAdamOptimizer::SaveSnapshot()
{
	if (SnapshotDirectory.IsEmpty())
	{
		return false;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*SnapshotDirectory))
	{
		PlatformFile.CreateDirectoryTree(*SnapshotDirectory);
	}

	const FString FilePath = FPaths::Combine(*SnapshotDirectory, TEXT("adam_params.csv"));
	FString Snapshot;
	Snapshot += FormatVector(FirstMoment) + TEXT("\n");
	Snapshot += FormatVector(SecondMoment) + TEXT("\n");
	if (bUseAMSGrad)
	{
		Snapshot += FormatVector(MaxSecondMoment) + TEXT("\n");
	}
	Snapshot += FString::FromInt(TimeStep) + TEXT("\n");

	if (!FFileHelper::SaveStringToFile(Snapshot, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append | FILEWRITE_NoFail))
	{
		UE_LOG(LogAdamOptimizer, Warning, TEXT("Failed to append Adam snapshot to %s"), *FilePath);
		return false;
	}

	return true;
}

float FAdamOptimizer::ComputeNorm(const TArray<float>& Vector)
{
	float Sum = 0.0f;
	for (float Value : Vector)
	{
		Sum += Value * Value;
	}

	return FMath::Sqrt(Sum);
}

FString FAdamOptimizer::FormatVector(const TArray<float>& Vector) const
{
	TArray<FString> StringValues;
	StringValues.Reserve(Vector.Num());
	for (float Value : Vector)
	{
		StringValues.Add(FString::SanitizeFloat(Value));
	}

	return FString::Join(StringValues, TEXT(","));
}

TArray<float> FAdamOptimizer::ParseVectorLine(const FString& Line) const
{
	TArray<FString> Tokens;
	Line.ParseIntoArray(Tokens, TEXT(","), true);

	TArray<float> Result;
	Result.Reserve(Tokens.Num());

	for (const FString& Token : Tokens)
	{
		Result.Add(FCString::Atof(*Token));
	}

	return Result;
}
