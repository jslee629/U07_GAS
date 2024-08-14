#include "CAttributeComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	//initialize variables
	Health = 100.f;
}


void UCAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

bool UCAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

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



