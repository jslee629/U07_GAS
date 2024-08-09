#include "CPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CInteractionComponent.h"

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

	//Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
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
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM(GetControlRotation(), HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	if (ensure(MagicBallClass))
	{
		GetWorld()->SpawnActor<AActor>(MagicBallClass, SpawnTM, SpawnParams);
	}
}

void ACPlayer::PrimaryInteraction()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteraction();
	}
}

