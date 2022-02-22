#include "Boids/Systems/Persistent/BoidsAlignmentSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Components/ImpulseComponent.h"
#include "Boids/Resources/BoidsSettings.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Shared/Utils.h"

void BoidsAlignmentSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunAlignment == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}
	const auto PerceptionRange = BoidsSettings->AlignmentPerceptionRange;
	const auto VisibilityConeAngleDegrees = BoidsSettings->VisibilityConeAngleDegrees;
	const auto AlignmentFactor = BoidsSettings->AlignmentFactor;

	Systems.Query<UImpulseComponent, UBoidComponent>().ForEach(
		[&](const auto& QueryItem)
		{
			const auto* Actor = QueryItem.Get<0>();
			auto* Impulse = QueryItem.Get<1>();
			const auto Position = Actor->GetActorLocation();
			const auto Direction = Actor->GetActorForwardVector();

			auto Count = 0;
			const auto Accum = Systems.Query<UImpulseComponent, UBoidComponent>()
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
										   const auto* OtherImpulse = QueryItem.Get<1>();

										   ++Count;
										   return OtherImpulse->Value;
									   })
								   .Sum(FVector(0));

			if (Count > 0)
			{
				const auto Average = Accum / static_cast<float>(Count);
				Impulse->Value += (Average - Impulse->Value) * AlignmentFactor;
			}
		});
}
