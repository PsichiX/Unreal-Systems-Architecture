#include "Multiplayer/Systems/Persistent/MultiplayerCorrectTransformSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Multiplayer/Components/MultiplayerCorrectTransformComponent.h"

void MultiplayerCorrectTransformSystem(USystemsWorld& Systems)
{
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

	Systems.Query<UMultiplayerCorrectTransformComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* CorrectTransform = QueryItem.Get<1>();
			CorrectTransform->Metronome.Progress(DeltaTime);

			if (CorrectTransform->Metronome.ConsumeTicked())
			{
				const auto* Actor = QueryItem.Get<0>();
				const auto Position = Actor->GetActorLocation();
				const auto Rotation = Actor->GetActorRotation();

				CorrectTransform->ClientUpdate(Position, Rotation);
			}
		});
}
