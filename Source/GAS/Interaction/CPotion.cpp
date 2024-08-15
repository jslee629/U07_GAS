#include "CPotion.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

ACPotion::ACPotion()
{
	//Initialize variables
	Cooltime = 10.f;
	bCanUse = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
}

void ACPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (bCanUse)
	{
		bCanUse = false;

		UCAttributeComponent* AttributeComp = Cast<UCAttributeComponent>(InstigatorPawn->GetComponentByClass(UCAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(100.f);
			GetWorldTimerManager().SetTimer(TimerHandle_ReturnToUse, this, &ACPotion::ReturnToUse, Cooltime, false);
		}
	}
}

void ACPotion::ReturnToUse_Implementation()
{
	bCanUse = true;
}


