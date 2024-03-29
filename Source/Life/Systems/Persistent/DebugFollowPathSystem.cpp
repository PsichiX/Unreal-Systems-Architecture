#include "Life/Systems/Persistent/DebugFollowPathSystem.h"

#include "DrawDebugHelpers.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/FollowPathComponent.h"
#include "Life/Resources/LifeDebugSystems.h"

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

	for (const auto& QueryItem : Systems.Query<UFollowPathComponent>())
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
	}
}
