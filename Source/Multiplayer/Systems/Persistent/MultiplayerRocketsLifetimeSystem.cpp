#include "Multiplayer/Systems/Persistent/MultiplayerRocketsLifetimeSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Multiplayer/Components/MultiplayerRocketComponent.h"
#include "Multiplayer/Resources/MultiplayerCommands.h"

void MultiplayerRocketsLifetimeSystem(USystemsWorld& Systems)
{
	auto* Commands = Systems.Resource<UMultiplayerCommands>();
	if (IsValid(Commands) == false)
	{
		return;
	}

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

	Systems.Query<UMultiplayerRocketComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			auto* Rocket = QueryItem.Get<1>();

			Rocket->LifetimeLeft -= DeltaTime;
			if (Rocket->LifetimeLeft <= 0)
			{
				Commands->RequestedActorsToKill.Enqueue(Actor);
			}
		});
}
