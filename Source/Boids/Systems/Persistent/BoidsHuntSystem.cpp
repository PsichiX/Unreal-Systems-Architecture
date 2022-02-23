#include "Boids/Systems/Persistent/BoidsHuntSystem.h"

#include "Shared/Components/FoodComponent.h"
#include "Shared/Components/HungerComponent.h"
#include "Shared/Utils.h"
#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Components/ImpulseComponent.h"
#include "Boids/Resources/BoidsSettings.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"

struct Meta
{
	float Distance = 0;
	FVector Position = FVector(0);
};

void BoidsHuntSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunHunt == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}
	const auto PerceptionRange = BoidsSettings->HuntPerceptionRange;
	const auto VisibilityConeAngleDegrees = BoidsSettings->VisibilityConeAngleDegrees;
	const auto HuntFactor = BoidsSettings->HuntFactor;

	Systems.Query<UHungerComponent, UImpulseComponent, UBoidComponent>().ForEach(
		[&](auto& QueryItem)
		{
			const auto* Actor = QueryItem.Get<0>();
			const auto* Hunger = QueryItem.Get<1>();
			auto* Impulse = QueryItem.Get<2>();
			const auto Position = Actor->GetActorLocation();
			const auto Direction = Actor->GetActorForwardVector();

			const auto Found =
				Systems.Query<UFoodComponent>()
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
					.Map<Meta>(
						[&](const auto& QueryItem)
						{
							const auto* OtherActor = QueryItem.Get<0>();
							const auto OtherPosition = OtherActor->GetActorLocation();
							const auto Distance = FVector::Distance(Position, OtherPosition);

							return Meta{Distance, OtherPosition};
						})
					.ComparedBy(
						[](const auto& A, const auto& B) { return A.Distance < B.Distance; });

			if (Found.IsSet())
			{
				const auto Desired = Found.GetValue().Position - Position;
				Impulse->Value += (Desired - Impulse->Value) * HuntFactor;
			}
		});
}
