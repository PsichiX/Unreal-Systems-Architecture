#include "Systems_Workspace/Life/Systems/Persistent/AgingSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/AgeComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"

void AgingSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto TimePassed = Settings->TimeScale * DeltaTime;

	Systems.Query<UAgeComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Age = QueryItem.Get<1>();

			Age->Value += TimePassed;
		});
}
