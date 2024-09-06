#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/CGameplayInterface.h"
#include "CChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class GAS_API ACChest : public AActor, public ICGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ACChest();

protected:
	virtual void BeginPlay() override;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;
	void OnActorLoaded_Implementation() override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxPitch;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* LidMesh;

	UPROPERTY(BlueprintReadOnly, SaveGame, ReplicatedUsing = "OnRep_LidOpen")
	bool bLidOpen;

	UFUNCTION()
	void OnRep_LidOpen();
};
