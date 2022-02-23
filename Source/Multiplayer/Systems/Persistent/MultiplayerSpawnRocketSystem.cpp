#include "Multiplayer/Systems/Persistent/MultiplayerSpawnRocketSystem.h"

#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Multiplayer/Components/MultiPlayerComponent.h"
#include "Multiplayer/Components/MultiplayerLocalControlComponent.h"
#include "Multiplayer/Components/MultiplayerSpawnRocketComponent.h"
#include "Multiplayer/Resources/MultiPlayerInput.h"
#include "Multiplayer/Resources/MultiplayerCommands.h"

void MultiplayerSpawnRocketSystem(USystemsWorld& Systems)
{
	auto* PlayerInput = Systems.Resource<UMultiPlayerInput>();
	if (IsValid(PlayerInput) == false)
	{
		return;
	}

	Systems
		.Query<UMultiplayerSpawnRocketComponent,
			UMultiPlayerComponent,
			UMultiplayerLocalControlComponent>()
		.ForEach(
			[&](auto& QueryItem)
			{
				if (PlayerInput->bAction)
				{
					PlayerInput->bAction = false;
					auto* SpawnRocket = QueryItem.Get<1>();

					SpawnRocket->ServerExecute();
				}
			});
}

void MultiplayerSpawnRocketCooldownSystem(USystemsWorld& Systems)
{
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

	Systems.Query<UMultiplayerSpawnRocketComponent, UMultiPlayerComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* SpawnRocket = QueryItem.Get<1>();

			SpawnRocket->Cooldown -= DeltaTime;
		});
}

void MultiplayerExecuteRocketSpawnsSystem(USystemsWorld& Systems)
{
	auto* Commands = Systems.Resource<UMultiplayerCommands>();
	if (IsValid(Commands) == false)
	{
		return;
	}

	AActor* Actor = nullptr;
	while (Commands->RocketsToSpawnForActors.Dequeue(Actor))
	{
		if (IsValid(Actor))
		{
			const auto Position = Actor->GetActorLocation();
			const auto Rotation = Actor->GetActorRotation();
			auto Params = FActorSpawnParameters();

			auto* Pawn = Cast<APawn>(Actor);
			if (IsValid(Pawn))
			{
				Params.Instigator = Pawn;
			}

			auto* Rocket = Systems.GetWorld()->SpawnActor<AActor>(
				Commands->RocketClass, Position, Rotation, Params);

			if (IsValid(Rocket))
			{
				auto* Velocity = Rocket->FindComponentByClass<UVelocityComponent>();
				if (IsValid(Velocity))
				{
					Velocity->Value =
						Rocket->GetActorForwardVector() * Commands->RocketInitialSpeed;
				}
			}
		}
	}
}
