#include "Boids/Systems/Persistent/BoidsCohesionSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Components/ImpulseComponent.h"
#include "Boids/Resources/BoidsSettings.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Shared/Utils.h"

void BoidsCohesionSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunCohesion == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}
	const auto PerceptionRange = BoidsSettings->CohesionPerceptionRange;
	const auto VisibilityConeAngleDegrees = BoidsSettings->VisibilityConeAngleDegrees;
	const auto CohesionFactor = BoidsSettings->CohesionFactor;

	Systems.Query<UImpulseComponent, UBoidComponent>().ForEach(
		[&](const auto& QueryItem)
		{
			const auto* Actor = QueryItem.Get<0>();
			auto* Impulse = QueryItem.Get<1>();
			const auto Position = Actor->GetActorLocation();
			const auto Direction = Actor->GetActorForwardVector();

			auto Count = 0;
			const auto Accum = Systems.Query<UBoidComponent>()
								   .Filter(
									   [&](const auto& QueryItem)
									   {
										   const auto* OtherActor = QueryItem.Get<0>();
										   const auto OtherPosition =
											   OtherActor->GetActorLocation();

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

										   ++Count;
										   return OtherActor->GetActorLocation();
									   })
								   .Sum(FVector(0));

			if (Count > 0)
			{
				const auto CenterOfMass = Accum / static_cast<float>(Count);
				const auto Desired = CenterOfMass - Position;
				Impulse->Value += (Desired - Impulse->Value) * CohesionFactor;
			}
		});
}
