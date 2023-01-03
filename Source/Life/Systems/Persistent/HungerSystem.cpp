#include "Life/Systems/Persistent/HungerSystem.h"

#include "Shared/Components/HungerComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/Resources/LifeSettings.h"

void HungerSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto TimePassed = Settings->TimeScale * DeltaTime;

	for (auto& QueryItem : Systems.Query<UHungerComponent>())
	{
		auto* Hunger = QueryItem.Get<1>();
		Hunger->Value += TimePassed;
	}
}
