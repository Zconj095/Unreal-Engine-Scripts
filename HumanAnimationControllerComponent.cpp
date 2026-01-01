#include "HumanAnimationControllerComponent.h"

#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"

UHumanAnimationControllerComponent::UHumanAnimationControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHumanAnimationControllerComponent::PlayAnimationByName(FName AnimationName, bool bLoop)
{
	if (!AnimationDatabase || !TargetMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("HumanAnimationController: Missing AnimationDatabase or TargetMesh."));
		return;
	}

	FAnimationMetadata Meta;
	if (AnimationDatabase->FindAnimationByName(AnimationName, Meta) && Meta.AnimationClip.IsValid())
	{
		UAnimSequenceBase* Sequence = Meta.AnimationClip.LoadSynchronous();
		PlaySequence(Sequence, bLoop);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation '%s' not found in the database."), *AnimationName.ToString());
	}
}

void UHumanAnimationControllerComponent::PlayAnimationByTag(FName Tag, bool bLoop)
{
	if (!AnimationDatabase || !TargetMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("HumanAnimationController: Missing AnimationDatabase or TargetMesh."));
		return;
	}

	TArray<FAnimationMetadata> Matches = AnimationDatabase->FindAnimationsByTag(Tag);
	if (Matches.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, Matches.Num() - 1);
		UAnimSequenceBase* Sequence = Matches[Index].AnimationClip.LoadSynchronous();
		PlaySequence(Sequence, bLoop);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No animations with tag '%s' found."), *Tag.ToString());
	}
}

void UHumanAnimationControllerComponent::PlaySequence(UAnimSequenceBase* Sequence, bool bLoop)
{
	if (!TargetMesh || !Sequence)
	{
		return;
	}

	TargetMesh->PlayAnimation(Sequence, bLoop);
}
