#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction_Effect.h"
#include "CAction_Effect_Thorns.generated.h"

class UCAttributeComponent;

UCLASS()
class GAS_API UCAction_Effect_Thorns : public UCAction_Effect
{
	GENERATED_BODY()

public:
	UCAction_Effect_Thorns();

	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewValue, float Delta);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Thorns", meta = (ClampMax = 1.00, ClampMin = 0.00, ContentDir, DisplayPriority = 9))
	float ReflectRatio;
};
