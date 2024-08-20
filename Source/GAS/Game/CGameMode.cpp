#include "CGameMode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"
#include "Characters/CBot.h"
#include "Components/CAttributeComponent.h"

ACGameMode::ACGameMode()
{
	SpawnTimerDelay = 2.f;
}

void ACGameMode::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ACGameMode::SpawnBotTimerElapsed, SpawnTimerDelay, true);
}

void ACGameMode::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotsQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACGameMode::OnQueryFinished);
	}
}

void ACGameMode::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn bots EQS query failed!"));
		return;
	}

	//count bots
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ACBot> It(GetWorld()); It; ++It)
	{
		ACBot* Bot = *It;

		UCAttributeComponent* AttributeComp = Cast<UCAttributeComponent>(Bot->GetComponentByClass(UCAttributeComponent::StaticClass()));
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots += 1;
		}
	}

	float MaxBotCount = 10.f;
	if (SpawnCurve)
	{
		MaxBotCount = SpawnCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	if (NrOfAliveBots >= (int32)MaxBotCount)
	{
		return;
	}

	//start Spawn bots
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		if (!ensure(BotClass))
		{
			UE_LOG(LogTemp, Error, TEXT("Bot Class Ref is not set!"));
			return;
		}
		GetWorld()->SpawnActor<AActor>(BotClass, Locations[0], FRotator::ZeroRotator);
	}
}
