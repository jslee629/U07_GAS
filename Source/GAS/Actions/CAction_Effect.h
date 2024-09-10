#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_Effect.generated.h"

UCLASS()
class GAS_API UCAction_Effect : public UCAction
{
	GENERATED_BODY()

public:
	UCAction_Effect();

public:
	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;
	
	UFUNCTION(BlueprintCallable)
	float GetRemainTime() const;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodEffect(AActor* Instigator);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	float Duration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	float Period;

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	UParticleSystem* EffectParticle;
};
