#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/CGameplayInterface.h"
#include "CPotion.generated.h"

class UStaticMeshComponent;

UCLASS()
class GAS_API ACPotion : public AActor, public ICGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ACPotion();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsCanUse() const { return bCanUse; }

	void Interact_Implementation(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent)
	void ReturnToUse();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

private:
	float Cooltime;
	bool bCanUse;
	FTimerHandle TimerHandle_ReturnToUse;
};
