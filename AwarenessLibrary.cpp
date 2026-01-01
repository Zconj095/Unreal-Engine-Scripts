#include "AwarenessLibrary.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"

bool UAwarenessLibrary::IsActorInSight(const AActor* Observer, const AActor* Target, float FOVDegrees)
{
    if (!Observer || !Target || FOVDegrees <= 0.f)
    {
        return false;
    }

    const FVector ObsLoc = Observer->GetActorLocation();
    const FVector TgtLoc = Target->GetActorLocation();
    const FVector ToTarget = TgtLoc - ObsLoc;

    // If target is essentially at the same position, treat as seen (Unity's Angle with zero vector behaves benignly)
    if (ToTarget.SizeSquared() <= KINDA_SMALL_NUMBER)
    {
        return true;
    }

    const FVector Forward = Observer->GetActorForwardVector().GetSafeNormal();
    const FVector Dir = ToTarget.GetSafeNormal();

    const float Dot = FMath::Clamp(FVector::DotProduct(Forward, Dir), -1.0f, 1.0f);
    const float AngleDeg = FMath::RadiansToDegrees(FMath::Acos(Dot));
    return AngleDeg < FOVDegrees;
}

