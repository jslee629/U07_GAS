#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCInteractionComponent;
class UAnimMontage;

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
	void PrimaryInteraction();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCInteractionComponent* InteractionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<AActor> MagicBallClass;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
	float AttackDelay;

private:
	FTimerHandle TimerHandle_PrimaryAction;
};
