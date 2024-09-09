#include "CPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	
	OnPawnChanged.Broadcast(InPawn);
}

void ACPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ACPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateChanged.Broadcast(PlayerState);
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Escape", IE_Pressed, this, &ACPlayerController::ToggleGameMenu);
}

void ACPlayerController::ToggleGameMenu()
{
	if (GameWidget && GameWidget->IsInViewport())
	{
		GameWidget->RemoveFromParent();
		GameWidget = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		return;
	}

	GameWidget = CreateWidget<UUserWidget>(this, GameWidgetClass);
	if (GameWidget)
	{
		GameWidget->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}
