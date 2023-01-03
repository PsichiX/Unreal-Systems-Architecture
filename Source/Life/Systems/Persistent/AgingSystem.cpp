#include "Life/Systems/Persistent/AgingSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/AgeComponent.h"
#include "Life/Resources/LifeSettings.h"

void AgingSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto TimePassed = Settings->TimeScale * DeltaTime;

	for (auto& QueryItem : Systems.Query<UAgeComponent>())
	{
		auto* Age = QueryItem.Get<1>();
		Age->Value += TimePassed;
	}
}
