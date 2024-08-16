#include "CPotion.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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
		UCAttributeComponent* AttributeComp = Cast<UCAttributeComponent>(InstigatorPawn->GetComponentByClass(UCAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			if (AttributeComp->Health < AttributeComp->MaxHealth)
			{
				bCanUse = false;

				AttributeComp->ApplyHealthChange(100.f);
				GetWorldTimerManager().SetTimer(TimerHandle_ReturnToUse, this, &ACPotion::ReturnToUse, Cooltime, false);
				DrawDebugString(GetWorld(), GetActorLocation(), TEXT("꿀꺽"), 0, FColor::White, 1.f, true, 4.f);
				MeshComp->SetScalarParameterValueOnMaterials("bCanUse", 0.1f);
			}
			else
			{
				DrawDebugString(GetWorld(), GetActorLocation(), TEXT("있었는데요"), 0, FColor::White, 1.f, true, 4.f);
			}
		}
	}
	else
	{
		DrawDebugString(GetWorld(), GetActorLocation(), TEXT("없어졌습니다"), 0, FColor::White, 1.f, true, 4.f);
	}
}

void ACPotion::ReturnToUse_Implementation()
{
	bCanUse = true;
	DrawDebugString(GetWorld(), GetActorLocation(), TEXT("다시 있네요^^"), 0, FColor::White, 1.f, true, 4.f);
	MeshComp->SetScalarParameterValueOnMaterials("bCanUse", 1.f);
}


