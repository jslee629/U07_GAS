#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, class UCAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UCAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastHealthChange(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRage() const;

	UFUNCTION(BlueprintCallable, Category = "Cheat")
	bool Kill(AActor* InstigatorActor);

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attributes")
	float Rage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attributes")
	float MaxRage;
};
