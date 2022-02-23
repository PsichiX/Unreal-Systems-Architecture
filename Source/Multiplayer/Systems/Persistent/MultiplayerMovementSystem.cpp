#include "Multiplayer/Systems/Persistent/MultiplayerMovementSystem.h"

#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsWorld.h"

void MultiplayerMovementSystem(USystemsWorld& Systems)
{
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

	Systems.Query<UVelocityComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto Velocity = QueryItem.Get<1>()->Value;
			const auto Position = Actor->GetActorLocation();

			Actor->SetActorLocation(Position + Velocity * DeltaTime);
		});
}
