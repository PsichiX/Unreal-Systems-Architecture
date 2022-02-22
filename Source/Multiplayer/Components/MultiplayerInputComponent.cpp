#include "Multiplayer/Components/MultiplayerInputComponent.h"

UMultiplayerInputComponent::UMultiplayerInputComponent()
{
	SetNetAddressable();
	SetIsReplicatedByDefault(true);
}

void UMultiplayerInputComponent::ServerUpdate_Implementation(FVector2D InRelativeMovement,
	FVector2D InRelativeLook)
{
	this->RelativeMovement = InRelativeMovement;
	this->RelativeLook = InRelativeLook;
}
