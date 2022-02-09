#include "Systems_Workspace/Life/Systems/Persistent/FollowPathSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/FlatMovementComponent.h"
#include "Systems_Workspace/Life/Components/FollowPathComponent.h"
#include "Systems_Workspace/Life/Components/SpeedComponent.h"

void FollowPathSystem(USystemsWorld& Systems)
{
	Systems.Query<UFlatMovementComponent, UFollowPathComponent, USpeedComponent>().ForEach(
		[&](auto& QueryItem)
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
		});
}
