#include "CPickupBase.h"
#include "Components/SphereComponent.h"

ACPickupBase::ACPickupBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Pickup");
	RootComponent = SphereComp;

	RespawnTime = 10.f;
}

void ACPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{

}

void ACPickupBase::Show()
{
	SetPickupState(true);
}

void ACPickupBase::HideAndCooldown()
{
	SetPickupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ACPickupBase::Show, RespawnTime);
}

void ACPickupBase::SetPickupState(bool bNewActive)
{
	SetActorEnableCollision(bNewActive);
	RootComponent->SetVisibility(bNewActive, true);
}
