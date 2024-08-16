#include "CAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

