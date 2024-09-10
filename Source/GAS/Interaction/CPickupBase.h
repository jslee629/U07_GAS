#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/CGameplayInterface.h"
#include "CPickupBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS(Abstract)
class GAS_API ACPickupBase : public AActor, public ICGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ACPickupBase();

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION()
	void Show();

	void HideAndCooldown();

	void SetPickupState(bool bNewActive);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();
};
