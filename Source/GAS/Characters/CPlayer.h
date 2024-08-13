#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCInteractionComponent;
class UAnimMontage;
class UCAttributeComponent;

UCLASS()
class GAS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void PrimaryAction();
	void PrimaryAction_TimeElapsed();

	void SecondaryAction();
	void SecondaryAction_TimeElapsed();

	void TertiaryAction();
	void TertiaryAction_TimeElapsed();

	void PrimaryInteraction();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCInteractionComponent* InteractionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<AActor> MagicBallClass;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<AActor> WarpBallClass;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<AActor> BlackHoleClass;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
	float AttackDelay;

private:
	FTimerHandle TimerHandle_PrimaryAction;
	FTimerHandle TimerHandle_SecondaryAction;
	FTimerHandle TimerHandle_TertiaryAction;
};
