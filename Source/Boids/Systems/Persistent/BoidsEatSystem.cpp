#include "Boids/Systems/Persistent/BoidsEatSystem.h"

#include "Shared/Components/FoodComponent.h"
#include "Shared/Components/HungerComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Components/RadiusComponent.h"
#include "Boids/Resources/BoidsSettings.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"

const float CONSUMPTION_SCALE = 10;

struct Meta
{
	double Distance = 0;
	UFoodComponent* Food = nullptr;
};

void BoidsEatSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunEat == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}
	const auto TimeScale = BoidsSettings->TimeScale;
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds() * TimeScale;

	Systems.Query<UHungerComponent, URadiusComponent, UBoidComponent>().ForEach(
		[&](auto& QueryItem)
		{
			const auto* Actor = QueryItem.Get<0>();
			auto* Hunger = QueryItem.Get<1>();
			const auto* Radius = QueryItem.Get<2>();
			const auto Position = Actor->GetActorLocation();

			auto Found =
				Systems.Query<UFoodComponent, URadiusComponent>()
					.FilterMap<Meta>(
						[&](const auto& QueryItem)
						{
							const auto* OtherActor = QueryItem.Get<0>();
							auto* Food = QueryItem.Get<1>();
							const auto* OtherRadius = QueryItem.Get<2>();
							const auto OtherPosition = OtherActor->GetActorLocation();
							const auto Distance = FVector::Distance(Position, OtherPosition);

							return Distance <= Radius->Value + OtherRadius->Value
								? TOptional<Meta>({Distance, Food})
								: TOptional<Meta>();
						})
					.ComparedBy(
						[](const auto& A, const auto& B) { return A.Distance < B.Distance; });

			if (Found.IsSet())
			{
				const auto Value = DeltaTime * CONSUMPTION_SCALE;

				Found.GetValue().Food->Amount -= Value;
				Hunger->Value -= Value;
			}
		});

	Systems.Query<UFoodComponent>().ForEach(
		[](const auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto* Food = QueryItem.Get<1>();

			if (Food->Amount <= 0)
			{
				Actor->Destroy();
			}
		});
}
