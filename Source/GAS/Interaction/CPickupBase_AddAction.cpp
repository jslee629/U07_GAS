#include "CPickupBase_AddAction.h"
#include "Components/CActionComponent.h"

void ACPickupBase_AddAction::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if (!ensure(ActionToGrantClass && InstigatorPawn))
	{
		return;
	}

	UCActionComponent* ActionComp = InstigatorPawn->GetComponentByClass(UCActionComponent::StaticClass());
	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrantClass))
		{
			FString Msg = FString::Printf(TEXT("%s is already learned"), *GetNameSafe(ActionToGrantClass));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, Msg);
		}

		ActionComp->AddAction(InstigatorPawn, ActionToGrantClass);
		HideAndCooldown();
	}
}