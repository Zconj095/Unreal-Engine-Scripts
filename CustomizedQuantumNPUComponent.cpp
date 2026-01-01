#include "CustomizedQuantumNPUComponent.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/DateTime.h"

UCustomizedQuantumNPUComponent::UCustomizedQuantumNPUComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCustomizedQuantumNPUComponent::BeginPlay()
{
	Super::BeginPlay();
	const FString LearningDirectory = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("LLM_Text"));
	TrainFromDirectory(LearningDirectory);
}

void UCustomizedQuantumNPUComponent::TrainFromDirectory(const FString& DirectoryPath)
{
	if (!FPaths::DirectoryExists(DirectoryPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Directory not found: %s"), *DirectoryPath);
		return;
	}

	TArray<FString> Files;
	IFileManager::Get().FindFiles(Files, *DirectoryPath, TEXT("*.txt"));

	for (const FString& FileName : Files)
	{
		FString FullPath = FPaths::Combine(DirectoryPath, FileName);
		FString Contents;
		if (FFileHelper::LoadFileToString(Contents, *FullPath))
		{
			AnalyzeTextFile(FullPath, Contents);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Training Completed. Extracted %d files."), ExtractedData.Num());
}

void UCustomizedQuantumNPUComponent::AnalyzeTextFile(const FString& FilePath, const FString& Contents)
{
	TArray<FString> Results;
	if (IsCode(Contents))
	{
		Results = ExtractCodeFeatures(Contents);
	}
	else
	{
		Results = ExtractTextPatterns(Contents);
	}

	ExtractedData.Add(FilePath, Results);
	UE_LOG(LogTemp, Log, TEXT("Processed: %s"), *FilePath);
}

bool UCustomizedQuantumNPUComponent::IsCode(const FString& Text) const
{
	static const TArray<FString> Keywords = { TEXT("class "), TEXT("function "), TEXT("var "), TEXT("def "), TEXT("public "), TEXT("private "), TEXT("namespace ") };
	for (const FString& Keyword : Keywords)
	{
		if (Text.Contains(Keyword))
		{
			return true;
		}
	}
	return false;
}

TArray<FString> UCustomizedQuantumNPUComponent::ExtractCodeFeatures(const FString& Code) const
{
	TArray<FString> Features;
	TArray<FString> Lines;
	Code.ParseIntoArrayLines(Lines);

	for (const FString& Line : Lines)
	{
		if (Line.Contains(TEXT("class ")))
		{
			Features.Add(FString::Printf(TEXT("Class: %s"), *Line));
		}
		if (Line.Contains(TEXT("function ")) || Line.Contains(TEXT("def ")) || Line.Contains(TEXT("void ")))
		{
			Features.Add(FString::Printf(TEXT("Method: %s"), *Line));
		}
		if (Line.Contains(TEXT("=")) && !Line.Contains(TEXT("==")))
		{
			Features.Add(FString::Printf(TEXT("Variable Assignment: %s"), *Line));
		}
	}

	return Features;
}

TArray<FString> UCustomizedQuantumNPUComponent::ExtractTextPatterns(const FString& Text) const
{
	TArray<FString> Patterns;
	TMap<FString, int32> Frequency;
	TArray<FString> Words;
	Text.ParseIntoArray(Words, TEXT(" \n\r.,;:!?"), true);

	for (const FString& Word : Words)
	{
		Frequency.FindOrAdd(Word)++;
	}

	for (const auto& Pair : Frequency)
	{
		if (Pair.Value > 1)
		{
			Patterns.Add(FString::Printf(TEXT("Pattern Found: %s appears %d times."), *Pair.Key, Pair.Value));
		}
	}

	return Patterns;
}
