#include "Systems_Workspace/Life/Systems/Persistent/DebugFollowPathSystem.h"

#include "DrawDebugHelpers.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/FollowPathComponent.h"
#include "Systems_Workspace/Life/Resources/LifeDebugSystems.h"

const float ARROW_LENGTH = 100;
const float THICKNESS = 8;
const float OFFSET = 200;
const float TRACE_RANGE = 10000.0f;

void DebugFollowPathSystem(USystemsWorld& Systems)
{
	const auto* Debug = Systems.Resource<ULifeDebugSystems>();
	if (IsValid(Debug) == false || Debug->bShowFollowPaths == false)
	{
		return;
	}

	Systems.Query<UFollowPathComponent>().ForEach(
		[&](const auto& QueryItem)
		{
			const auto* Follow = QueryItem.Get<1>();

			for (const auto& Segment : Follow->PathSegmentsView())
			{
				auto StartPosition = FVector(Segment.StartPosition, OFFSET);
				auto EndPosition = FVector(Segment.EndPosition, OFFSET);

				FHitResult Hit;
				if (Systems.GetWorld()->LineTraceSingleByChannel(Hit,
						StartPosition + FVector(0, 0, TRACE_RANGE),
						StartPosition - FVector(0, 0, TRACE_RANGE),
						ECollisionChannel::ECC_GameTraceChannel1))
				{
					StartPosition.Z += Hit.ImpactPoint.Z;
				}
				if (Systems.GetWorld()->LineTraceSingleByChannel(Hit,
						EndPosition + FVector(0, 0, TRACE_RANGE),
						EndPosition - FVector(0, 0, TRACE_RANGE),
						ECollisionChannel::ECC_GameTraceChannel1))
				{
					EndPosition.Z += Hit.ImpactPoint.Z;
				}

				DrawDebugDirectionalArrow(Systems.GetWorld(),
					StartPosition,
					EndPosition,
					ARROW_LENGTH,
					FColor::Red,
					false,
					0,
					0,
					THICKNESS);
			}
		});
}
