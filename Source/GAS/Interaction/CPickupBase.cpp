#include "CPickupBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ACPickupBase::ACPickupBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Pickup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RespawnTime = 10.f;

	bIsActive = true;

	SetReplicates(true);
}

void ACPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{

}

FText ACPickupBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
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
	bIsActive = bNewActive;
	OnRep_IsActive();
}

void ACPickupBase::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ACPickupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPickupBase, bIsActive);
}