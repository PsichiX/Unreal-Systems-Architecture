#include "Multiplayer/Components/MultiplayerSpawnRocketComponent.h"

#include "Systems/Public/SystemsStatics.h"

#include "Multiplayer/Constants.h"
#include "Multiplayer/Resources/MultiplayerCommands.h"

UMultiplayerSpawnRocketComponent::UMultiplayerSpawnRocketComponent()
{
	SetNetAddressable();
	SetIsReplicatedByDefault(true);
}

void UMultiplayerSpawnRocketComponent::ServerExecute_Implementation()
{
	auto* Commands = USystemsStatics::GetResource<UMultiplayerCommands>(FName(), GetWorld());
	if (IsValid(GetOwner()) && IsValid(Commands) && this->Cooldown <= 0)
	{
		Commands->RocketsToSpawnForActors.Enqueue(GetOwner());
		this->Cooldown = Commands->RocketCooldown;
	}
}
