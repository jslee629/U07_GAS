#include "CBot.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CActionComponent.h"
#include "UI/CWorldWidget.h"

ACBot::ACBot()
{
	PawnSesningComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSesningComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UCActionComponent>("ActionComp");
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeToHit";
}

void ACBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSesningComp->OnSeePawn.AddDynamic(this, &ACBot::OnSeePawn);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACBot::OnHealthChanged);
}

void ACBot::OnSeePawn(APawn* Pawn)
{
	AAIController* AIC = GetController<AAIController>();
	if (ensure(AIC))
	{
		SetTargetActor(Pawn);

		if (SeePawnWidget == nullptr)
		{
			SeePawnWidget = CreateWidget<UCWorldWidget>(GetWorld(), SeePawnWidgetClass);
			SeePawnWidget->AttachToActor = this;
			SeePawnWidget->AddToViewport();
		}
		else if (SeePawnWidget)
		{
			SeePawnWidget->SetVisibility(ESlateVisibility::Visible);
		}

		if (SeePawnSound)
		{
			AudioComp->SetSound(SeePawnSound);
			AudioComp->Play();
		}
	}
}

void ACBot::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.f)
		{
			AAIController* AIC = GetController<AAIController>();
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(10.f);
			return;
		}

		if (HealthBarWidget == nullptr)
		{
			HealthBarWidget = CreateWidget<UCWorldWidget>(GetWorld(), HealthBarWidgetClass);
			if (HealthBarWidget)
			{
				HealthBarWidget->AttachToActor = this;
				HealthBarWidget->AddToViewport();
			}
		}
	}
}

void ACBot::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}
