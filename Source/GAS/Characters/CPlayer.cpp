#include "CPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CInteractionComponent.h"
#include "Components/CAttributeComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACPlayer::ACPlayer()
{
	//initialize variables
	AttackDelay = 0.2f;

	//Create Scene Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	//Create Actor Component
	InteractionComp = CreateDefaultSubobject<UCInteractionComponent>(TEXT("InteractionComp"));
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>(TEXT("AttributeComp"));

	//Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ACPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACPlayer::OnHealthChanged);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind Axis Event
	InputComponent->BindAxis("MoveForward", this, &ACPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACPlayer::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	//Bind Action Event
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("PrimaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::PrimaryAction);
	InputComponent->BindAction("PrimaryInteraction", EInputEvent::IE_Pressed, this, &ACPlayer::PrimaryInteraction);
	InputComponent->BindAction("SecondaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::SecondaryAction);
	InputComponent->BindAction("SecondaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::SecondaryAction);
	InputComponent->BindAction("TertiaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::TertiaryAction);
	InputComponent->BindAction("TertiaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::TertiaryAction);
}

void ACPlayer::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = GetController<APlayerController>();
		DisableInput(PC);
	}
}

void ACPlayer::MoveForward(float AxisValue)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0.f;

	AddMovementInput(ControlRot.Vector(), AxisValue);
}

void ACPlayer::MoveRight(float AxisValue)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0.f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, AxisValue);
}

void ACPlayer::PrimaryAction()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAction, this, &ACPlayer::PrimaryAction_TimeElapsed, AttackDelay);
}

void ACPlayer::PrimaryAction_TimeElapsed()
{
	
	SpawnProjectile(MagicBallClass);
}

void ACPlayer::SecondaryAction()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAction, this, &ACPlayer::SecondaryAction_TimeElapsed, AttackDelay);
}

void ACPlayer::SecondaryAction_TimeElapsed()
{
	SpawnProjectile(WarpBallClass);
}

void ACPlayer::TertiaryAction()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_TertiaryAction, this, &ACPlayer::TertiaryAction_TimeElapsed, AttackDelay);
}

void ACPlayer::TertiaryAction_TimeElapsed()
{
	SpawnProjectile(BlackHoleClass);
}

void ACPlayer::PrimaryInteraction()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteraction();
	}
}

void ACPlayer::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ClassToSpawn))
	{
		//Camera Trace
		FHitResult Hit;

		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000.f);

		FCollisionObjectQueryParams ObjectQueries;
		ObjectQueries.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueries.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueries.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueries, Shape, QueryParams))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		//Spawn Projectile
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		FTransform SpawnTM(ProjectileRotation, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}
