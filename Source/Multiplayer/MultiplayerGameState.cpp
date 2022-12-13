#include "Multiplayer/MultiplayerGameState.h"

void AMultiplayerGameState::BeginPlay()
{
	Super::BeginPlay();

	auto* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}
