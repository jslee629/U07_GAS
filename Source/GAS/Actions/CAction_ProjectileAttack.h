#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_ProjectileAttack.generated.h"

class UAnimMontage;

UCLASS()
class GAS_API UCAction_ProjectileAttack : public UCAction
{
	GENERATED_BODY()

public:
	UCAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

protected:
	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Action")
	float AttackDelay;

	UPROPERTY(EditAnywhere, Category = "Action")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
	UParticleSystem* MuzzleParticle;

	UPROPERTY(EditAnywhere, Category = "Action")
	FName HandSocketName;
};
