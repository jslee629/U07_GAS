#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBot.generated.h"

class UPawnSensingComponent;
class UCAttributeComponent;

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

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UPawnSensingComponent* PawnSesningComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

};
