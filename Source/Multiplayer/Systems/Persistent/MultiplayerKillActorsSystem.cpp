#include "Multiplayer/Systems/Persistent/MultiplayerKillActorsSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Multiplayer/Resources/MultiplayerCommands.h"

void MultiplayerKillActorsSystem(USystemsWorld& Systems)
{
	auto* Commands = Systems.Resource<UMultiplayerCommands>();
	if (IsValid(Commands) == false)
	{
		return;
	}

	AActor* Actor = nullptr;
	while (Commands->RequestedActorsToKill.Dequeue(Actor))
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
		}
	}
}
