#pragma once

#include "CoreMinimal.h"
#include "CProjectileBase.h"
#include "CMagicBall.generated.h"

UCLASS()
class GAS_API ACMagicBall : public ACProjectileBase
{
	GENERATED_BODY()
	
public:	
	ACMagicBall();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;
};
