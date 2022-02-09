#include "Systems_Workspace/Life/Systems/Persistent/HungerSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Resources/LifeSettings.h"
#include "Systems_Workspace/Shared/Components/HungerComponent.h"

void HungerSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto TimePassed = Settings->TimeScale * DeltaTime;

	Systems.Query<UHungerComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Hunger = QueryItem.Get<1>();

			Hunger->Value += TimePassed;
		});
}
