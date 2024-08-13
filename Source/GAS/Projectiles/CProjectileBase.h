#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CProjectileBase.generated.h"

class UParticleSystem;
class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class GAS_API ACProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACProjectileBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UParticleSystemComponent*  EffectComp;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

};
