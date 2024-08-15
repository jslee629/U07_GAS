#include "CAttributeComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	//initialize variables
	MaxHealth = 100.f;
	Health = MaxHealth;
}


void UCAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

bool UCAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	}

	return true;
}

bool UCAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}



