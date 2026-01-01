#include "FuzzyRecognitionComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Logging/LogMacros.h"

UFuzzyRecognitionComponent::UFuzzyRecognitionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFuzzyRecognitionComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeObjectsInScene();
	InitializeFuzzySets();
}

void UFuzzyRecognitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFuzzyRecognition();
	CheckRecognition();
}

void UFuzzyRecognitionComponent::InitializeObjectsInScene()
{
	// If user supplied objects, respect them.
	if (ObjectsInScene.Num() > 0)
	{
		return;
	}

	if (!GetWorld())
	{
		return;
	}

	// Spawn simple placeholder actors with random positions and tags.
	for (int32 i = 0; i < 3; ++i)
	{
		FActorSpawnParameters Params;
		Params.Name = *FString::Printf(TEXT("FuzzyObject%d"), i + 1);
		AActor* NewActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
		if (NewActor)
		{
			const float X = FMath::FRandRange(-1000.f, 1000.f);
			const float Y = FMath::FRandRange(-1000.f, 1000.f);
			const float Z = FMath::FRandRange(0.f, 1000.f);
			NewActor->SetActorLocation(FVector(X, Y, Z));

			const bool bRecognizable = (FMath::RandRange(0, 1) == 0);
			NewActor->Tags.Add(bRecognizable ? FName(TEXT("Recognizable")) : FName(TEXT("Unrecognizable")));

			ObjectsInScene.Add(NewActor);
		}
	}
}

void UFuzzyRecognitionComponent::InitializeFuzzySets()
{
	FuzzySets.Reset();
	for (AActor* Obj : ObjectsInScene)
	{
		const FString Name = Obj ? Obj->GetName() : TEXT("None");
		FuzzySets.Add(FFuzzySet(Name, 0.f));
	}
}

void UFuzzyRecognitionComponent::UpdateFuzzyRecognition()
{
	if (!GetOwner())
	{
		return;
	}

	for (int32 i = 0; i < ObjectsInScene.Num(); ++i)
	{
		AActor* Obj = ObjectsInScene[i];
		if (!Obj)
		{
			continue;
		}

		const float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Obj->GetActorLocation());
		const float Similarity = CalculateSimilarity(Obj);

		const float RecognitionDegree = GetRecognitionDegree(Distance, Similarity);
		if (FuzzySets.IsValidIndex(i))
		{
			FuzzySets[i] = FFuzzySet(Obj->GetName(), RecognitionDegree);
		}

		const FString TagsString = FString::JoinBy(Obj->Tags, TEXT(", "), [](const FName& Tag) { return Tag.ToString(); });
		UE_LOG(LogTemp, Log, TEXT("Recognition degree for %s (Tags: %s): %.2f"), *Obj->GetName(), *TagsString, RecognitionDegree);
	}
}

float UFuzzyRecognitionComponent::GetRecognitionDegree(float Distance, float Similarity) const
{
	const float DistanceDegree = FMath::Clamp(1.f - (Distance / DistanceFactor), 0.f, 1.f);
	const float SimilarityDegree = FMath::Clamp(Similarity, 0.f, 1.f);
	return FMath::Min(DistanceDegree, SimilarityDegree);
}

float UFuzzyRecognitionComponent::CalculateSimilarity(const AActor* Actor) const
{
	if (!Actor)
	{
		return 0.f;
	}

	if (Actor->Tags.Contains(FName(TEXT("Recognizable"))))
	{
		return 1.f;
	}
	if (Actor->Tags.Contains(FName(TEXT("Unrecognizable"))))
	{
		return 0.3f;
	}
	return 0.f;
}

void UFuzzyRecognitionComponent::CheckRecognition() const
{
	for (const FFuzzySet& Set : FuzzySets)
	{
		if (Set.DegreeOfMembership >= RecognitionThreshold)
		{
			UE_LOG(LogTemp, Log, TEXT("%s is recognized with a degree of %.2f"), *Set.Name, Set.DegreeOfMembership);
			// Hook: trigger additional behavior here.
		}
	}
}
