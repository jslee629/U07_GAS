#pragma once

#include "CoreMinimal.h"
#include "Projectiles/CProjectileBase.h"
#include "CWarpBall.generated.h"

UCLASS()
class GAS_API ACWarpBall : public ACProjectileBase
{
	GENERATED_BODY()

public:
	ACWarpBall();
	
protected:
	virtual void BeginPlay() override;

protected:
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Detonate")
	float DetonateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Detonate")
	float TeleportDelay;

	FTimerHandle TimeHandle_DelayDetonate;
};
