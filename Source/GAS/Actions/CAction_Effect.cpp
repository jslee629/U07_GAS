#include "CAction_Effect.h"
#include "Components/CActionComponent.h"

UCAction_Effect::UCAction_Effect()
{
	bAutoStart = true;
}

void UCAction_Effect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UCAction_Effect::StopAction_Implementation(AActor* Instigator)
{
	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle);
	if (RemainingTime < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect(Instigator);
	}

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	Super::StopAction_Implementation(Instigator);

	UCActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp)
	{
		ActionComp->RemoveAction(this);
	}
}

void UCAction_Effect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
}
