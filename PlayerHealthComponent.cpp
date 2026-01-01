#include "PlayerHealthComponent.h"

#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

UPlayerHealthComponent::UPlayerHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth; // Initialize with full health like Unity Start()

    if (bAutoBindToAnyDamage)
    {
        if (AActor* Owner = GetOwner())
        {
            Owner->OnTakeAnyDamage.AddDynamic(this, &UPlayerHealthComponent::HandleAnyDamage);
        }
    }

    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UPlayerHealthComponent::Heal(float Amount)
{
    if (Amount <= 0.f)
    {
        return;
    }

    const float Old = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);

    UE_LOG(LogTemp, Log, TEXT("Player healed. Current health: %0.2f"), CurrentHealth);

    if (!FMath::IsNearlyEqual(Old, CurrentHealth))
    {
        OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
    }
}

void UPlayerHealthComponent::TakeDamageAmount(float Amount)
{
    if (Amount <= 0.f)
    {
        return;
    }

    const float Old = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.f, MaxHealth);

    UE_LOG(LogTemp, Log, TEXT("Player took damage. Current health: %0.2f"), CurrentHealth);

    if (!FMath::IsNearlyEqual(Old, CurrentHealth))
    {
        OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
    }

    if (CurrentHealth <= 0.f)
    {
        Die();
    }
}

void UPlayerHealthComponent::HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* /*DamageType*/, AController* /*InstigatedBy*/, AActor* /*DamageCauser*/)
{
    TakeDamageAmount(Damage);
}

void UPlayerHealthComponent::Die()
{
    UE_LOG(LogTemp, Warning, TEXT("Player died!"));
    OnDeath.Broadcast();
}
