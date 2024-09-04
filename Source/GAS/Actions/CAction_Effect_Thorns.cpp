#include "CAction_Effect_Thorns.h"
#include "Components/CActionComponent.h"
#include "Components/CAttributeComponent.h"
#include "Game/CFunctionLibrary.h"

UCAction_Effect_Thorns::UCAction_Effect_Thorns()
{
	Duration = 0.f;
	Period = 0.f;
	ReflectRatio = 0.2f;
}

void UCAction_Effect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	AActor* Owner = GetOwningComponent()->GetOwner();
	UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttributes(Owner);
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UCAction_Effect_Thorns::OnHealthChanged);
	}
}

void UCAction_Effect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	AActor* Owner = GetOwningComponent()->GetOwner();
	UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttributes(Owner);
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UCAction_Effect_Thorns::OnHealthChanged);
	}
}

void UCAction_Effect_Thorns::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewValue, float Delta)
{
	AActor* Owner = GetOwningComponent()->GetOwner();
	if (Delta < 0.f && Owner != InstigatorActor)
	{
		int32 RoundDamage = FMath::RoundToInt(FMath::Abs(Delta) * ReflectRatio);
		if (RoundDamage == 0)
		{
			return;
		}

		UCFunctionLibrary::ApplyDamage(Owner, InstigatorActor, RoundDamage);
	}
}
