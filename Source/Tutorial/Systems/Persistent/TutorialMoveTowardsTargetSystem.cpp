#include "Tutorial/Systems/Persistent/TutorialMoveTowardsTargetSystem.h"

#include "Shared/Components/SpeedComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Tutorial/Components/TutorialMovementComponent.h"
#include "Tutorial/Components/TutorialTargetComponent.h"

void TutorialMoveTowardsTargetSystem(USystemsWorld& Systems)
{
	for (auto& QueryItem :
		Systems.Query<UTutorialMovementComponent, USpeedComponent, UTutorialTargetComponent>())
	{
		const auto* Actor = QueryItem.Get<0>();
		auto* Movement = QueryItem.Get<1>();
		const auto* Speed = QueryItem.Get<2>();
		const auto* Target = QueryItem.Get<3>();
		const auto Position = Actor->GetActorLocation();
		const auto Difference = FVector2D(Target->Position - Position);
		const auto Distance = FMath::Min(Difference.Size(), Speed->Value);

		if (Distance > 1)
		{
			Movement->Value += Difference.GetSafeNormal() * Distance;
		}
	}
}
