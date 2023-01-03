#include "Boids/Systems/Persistent/BoidsSeparationSystem.h"

#include "Shared/Utils.h"
#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Components/ImpulseComponent.h"
#include "Boids/Components/ObstacleComponent.h"
#include "Boids/Components/RadiusComponent.h"
#include "Boids/Resources/BoidsSettings.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"

void BoidsSeparationSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunSeparation == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}
	const auto PerceptionRange = BoidsSettings->SeparationPerceptionRange;
	const auto VisibilityConeAngleDegrees = BoidsSettings->VisibilityConeAngleDegrees;
	const auto SeparationFactor = BoidsSettings->SeparationFactor;

	for (const auto& QueryItem :
		Systems.Query<UImpulseComponent, URadiusComponent, UBoidComponent>())
	{
		const auto* Actor = QueryItem.Get<0>();
		auto* Impulse = QueryItem.Get<1>();
		const auto* Radius = QueryItem.Get<2>();
		const auto Position = Actor->GetActorLocation();
		const auto Direction = Actor->GetActorForwardVector();

		auto Count = 0;
		const auto Accum =
			Systems.Query<UObstacleComponent, URadiusComponent>()
				.Filter(
					[&](const auto& QueryItem)
					{
						const auto* OtherActor = QueryItem.Get<0>();
						const auto OtherPosition = OtherActor->GetActorLocation();

						return Actor != OtherActor &&
							IsInVisionSpace(Position,
								Direction,
								OtherPosition,
								PerceptionRange,
								VisibilityConeAngleDegrees);
					})
				.Map<FVector>(
					[&](const auto& QueryItem)
					{
						const auto* OtherActor = QueryItem.Get<0>();
						const auto* Obstacle = QueryItem.Get<1>();
						const auto* OtherRadius = QueryItem.Get<2>();
						const auto OtherPosition = OtherActor->GetActorLocation();
						const auto Diff = Position - OtherPosition;
						const auto Dist = Diff.Size() - Radius->Value - OtherRadius->Value;
						const auto DistSqr = Dist * Dist;

						++Count;
						return Dist > 0 ? (Diff * Obstacle->AvoidanceWeight) / DistSqr : FVector(0);
					})
				.Sum(FVector(0));

		if (Count > 0)
		{
			const auto Average = Accum / static_cast<float>(Count);
			Impulse->Value += (Average - Impulse->Value) * SeparationFactor;
		}
	}
}
