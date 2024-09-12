#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "CGameMode.generated.h"

class UEnvQuery;
class UCSaveGame;
class UCSpawnBotDataAsset;

USTRUCT(BlueprintType)
struct FSpawnBotRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSpawnBotRow()
	{
		Weight = 1.f;
		Cost = 5.f;
		KillReward = 20.f;
	}

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FPrimaryAssetId BotDataAssetID;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Weight;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Cost;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float KillReward;
};

UCLASS()
class GAS_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACGameMode();

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	//Kill
public:
	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Credits")
	int32 CreditsPerKill;

	//Spawn Bots
protected:
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerDelay;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UFUNCTION()
	void OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* SpawnCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* SpawnBotDataTable;

	void OnDataAssetLoaded(FPrimaryAssetId PrimaryAssetID, FTransform Transform);

	//Spawn Pickup
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	UEnvQuery* SpawnPickupQuery;

	UFUNCTION()
	void OnSpawnPickupQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	TArray<TSubclassOf<AActor>> PickupClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float MinimumPickupDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	int32 MaxPickupCount;

	//Save Game
protected:
	UPROPERTY()
	UCSaveGame* CurrentSaveGame;

	FString SlotName;

public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
};
