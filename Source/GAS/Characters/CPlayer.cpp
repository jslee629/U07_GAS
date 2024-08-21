#include "CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CInteractionComponent.h"
#include "Components/CAttributeComponent.h"

ACPlayer::ACPlayer()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UCInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	AttackDelay = 0.2f;
	HandSocketName = "Muzzle_01";
	TimeToHitParamName = "TimeToHit";
}

void ACPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ACPlayer::OnHealthChanged);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//AxisEvent
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//ActionEvent
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::PrimaryAction);
	PlayerInputComponent->BindAction("SecondaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::SecondaryAction);
	PlayerInputComponent->BindAction("ThirdAction", EInputEvent::IE_Pressed, this, &ACPlayer::ThirdAction);
	PlayerInputComponent->BindAction("PrimaryInteraction", EInputEvent::IE_Pressed, this, &ACPlayer::PrimaryInteraction);
}

void ACPlayer::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = GetController<APlayerController>();
		DisableInput(PC);
	}
}

void ACPlayer::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0.f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ACPlayer::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0.f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ACPlayer::PrimaryAction()
{
	PlayAttackAction();

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAction, this, &ACPlayer::PrimaryAction_TimeElapsed, AttackDelay);
}

void ACPlayer::PrimaryAction_TimeElapsed()
{
	SpawnProjectile(MagicBallClass);
}

void ACPlayer::SecondaryAction()
{
	PlayAttackAction();

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAction, this, &ACPlayer::SecondaryAction_TimeElapsed, AttackDelay);
}

void ACPlayer::SecondaryAction_TimeElapsed()
{
	SpawnProjectile(WarpBallClass);
}

void ACPlayer::ThirdAction()
{
	PlayAttackAction();

	GetWorldTimerManager().SetTimer(TimerHandle_ThirdAction, this, &ACPlayer::ThirdAction_TimeElapsed, AttackDelay);
}

void ACPlayer::ThirdAction_TimeElapsed()
{
	SpawnProjectile(BlackHoleClass);
}

void ACPlayer::PlayAttackAction()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	if (MuzzleParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	}
}

void ACPlayer::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams QueryParam;
		QueryParam.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjectQueries;
		ObjectQueries.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueries.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueries.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000.f);
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueries, Shape, QueryParam))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		
		FRotator ProjectionRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM(ProjectionRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ACPlayer::PrimaryInteraction()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteraction();
	}
}

