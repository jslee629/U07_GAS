#pragma once

#include "CoreMinimal.h"
#include "Interaction/CPickupBase.h"
#include "CSkillBook.generated.h"

class UCAction;

UCLASS()
class GAS_API ACSkillBook : public ACPickupBase
{
	GENERATED_BODY()
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UCAction> WarpClass;
};
