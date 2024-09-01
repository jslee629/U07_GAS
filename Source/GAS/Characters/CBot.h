#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBot.generated.h"

class UPawnSensingComponent;
class UCAttributeComponent;
class UCActionComponent;
class UCWorldWidget;
class USoundCue;

UCLASS()
class GAS_API ACBot : public ACharacter
{
	GENERATED_BODY()

public:
	ACBot();

protected:
	virtual void PostInitializeComponents() override;

private:
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTarget);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UPawnSensingComponent* PawnSesningComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Imapct")
	FName TimeToHitParamName;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UCWorldWidget* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SeePawnWidgetClass;

	UCWorldWidget* SeePawnWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	USoundCue* SeePawnSound;
};
