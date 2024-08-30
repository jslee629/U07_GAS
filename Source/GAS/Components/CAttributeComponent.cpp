#include "CAttributeComponent.h"
#include "CActionComponent.h"
#include "Game/CGameMode.h"

static TAutoConsoleVariable<float> CVarDamageMultiply(TEXT("Tore.DamageMultiply"), 1.f, TEXT("Modify damage multiply"), ECVF_Cheat);

UCAttributeComponent::UCAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
	MaxRage= 100.f;
	Rage = 0.f;
	RageIncreaseRate = 5.f;
}

void UCAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

UCAttributeComponent* UCAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCAttributeComponent>(FromActor->GetComponentByClass(UCAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UCAttributeComponent::IsActorAlive(AActor* Actor)
{
	UCAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

bool UCAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false;
	}

	if (Delta < 0.f)
	{
		float DamageMultiply = CVarDamageMultiply.GetValueOnGameThread();
		Delta *= DamageMultiply;

		ApplyRageChange(InstigatorActor, RageIncreaseRate);
	}

	float PrevHealth = Health;
	Health = FMath::Clamp(Health += Delta, 0.f, MaxHealth);

	float ActualDelta = Health - PrevHealth;

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	}

	if (ActualDelta < 0.f && Health <= 0.f)
	{
		ACGameMode* GM = GetWorld()->GetAuthGameMode<ACGameMode>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return !FMath::IsNearlyZero(ActualDelta);
}

bool UCAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool UCAttributeComponent::IsFullHealth() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

float UCAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UCAttributeComponent::GetHealth() const
{
	return Health;
}

float UCAttributeComponent::GetMaxRage() const
{
	return MaxRage;
}

float UCAttributeComponent::GetRage() const
{
	return Rage;
}

void UCAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	Rage = FMath::Clamp(Rage += Delta, 0.f, MaxRage);

	if (OnRageChanged.IsBound())
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, Delta);
	}
}

bool UCAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}