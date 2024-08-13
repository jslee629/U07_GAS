#include "CMagicBall.h"
#include "Components/SphereComponent.h"
#include "Components/CAttributeComponent.h"

ACMagicBall::ACMagicBall()
{
	SphereComp->SetSphereRadius(20.f);
	DamageAmount = 20.f;
}

void ACMagicBall::BeginPlay()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACMagicBall::OnActorOverlap);
	
	Super::BeginPlay();
}

void ACMagicBall::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UCAttributeComponent* AttributueComp = Cast<UCAttributeComponent>(OtherActor->GetComponentByClass(UCAttributeComponent::StaticClass()));
		if (AttributueComp)
		{
			AttributueComp->ApplyHealthChange(-DamageAmount);

			Explode();
		}
	}
}

