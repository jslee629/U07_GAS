#include "CAction.h"
#include "Components/CActionComponent.h"

UWorld* UCAction::GetWorld() const
{
	UCActionComponent* ActionComp = Cast<UCActionComponent>(GetOuter());
	if (ActionComp)
	{
		return ActionComp->GetWorld();
	}

	return nullptr;
}

void UCAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running : %s"), *GetNameSafe(this));

	UCActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped : %s"), *GetNameSafe(this));

	UCActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;
}

bool UCAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UCActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

bool UCAction::IsRunning() const
{
	return bIsRunning;
}

UCActionComponent* UCAction::GetOwningComponent() const
{
	return Cast<UCActionComponent>(GetOuter());
}
