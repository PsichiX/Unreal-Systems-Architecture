#include "Systems_Workspace/Boids/Systems/Persistent/BoidsHungerSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/Components/BoidComponent.h"
#include "Systems_Workspace/Boids/Resources/BoidsSettings.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Systems_Workspace/Shared/Components/HungerComponent.h"

void BoidsHungerSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunHunger == false)
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

	Systems.Query<UHungerComponent, UBoidComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Hunger = QueryItem.Get<1>();

			Hunger->Value += DeltaTime;
		});
}
