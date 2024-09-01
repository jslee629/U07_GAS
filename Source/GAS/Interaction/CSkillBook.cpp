#include "CSkillBook.h"
#include "Components/CActionComponent.h"

void ACSkillBook::Interact_Implementation(APawn* InstigatorPawn)
{
	UCActionComponent* ActionComp = Cast<UCActionComponent>(InstigatorPawn->GetComponentByClass(UCActionComponent::StaticClass()));
	if (ActionComp)
	{
		ActionComp->AddAction(InstigatorPawn, WarpClass);
		HideAndCooldown();
	}
}

