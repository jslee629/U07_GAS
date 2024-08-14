#include "CAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	ensure(BehaviorTree);
	RunBehaviorTree(BehaviorTree);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (PlayerPawn)
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	}
}

