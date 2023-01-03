#include "Life/Systems/Persistent/FollowPathSystem.h"

#include "Shared/Components/SpeedComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/FlatMovementComponent.h"
#include "Life/Components/FollowPathComponent.h"

void FollowPathSystem(USystemsWorld& Systems)
{
	for (auto& QueryItem :
		Systems.Query<UFlatMovementComponent, UFollowPathComponent, USpeedComponent>())
	{
		const auto* Actor = QueryItem.Get<0>();
		auto* Movement = QueryItem.Get<1>();
		const auto* Follow = QueryItem.Get<2>();
		const auto* Speed = QueryItem.Get<3>();
		const auto Position = FVector2D(Actor->GetActorLocation());

		if (const auto Location = Follow->FindLocationAlongPath(Position))
		{
			const auto NewLocation = Location.GetValue() + Speed->Value;
			if (const auto TargetPosition = Follow->SamplePositionAlongPath(NewLocation))
			{
				const auto Difference = TargetPosition.GetValue() - Position;
				Movement->Value += Difference.GetSafeNormal() * Speed->Value;
			}
		}
	}
}
