#include "Multiplayer/Components/MultiplayerLocalControlComponent.h"

bool UMultiplayerLocalControlComponent::CanBeRegisteredToSystemsWorld() const
{
	const auto* Pawn = Cast<APawn>(GetOwner());
	return IsValid(Pawn) && Pawn->IsLocallyControlled();
}
