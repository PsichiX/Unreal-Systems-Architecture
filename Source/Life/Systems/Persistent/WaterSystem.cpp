#include "Life/Systems/Persistent/WaterSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/WaterComponent.h"
#include "Life/Resources/LifeSettings.h"

void WaterSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto TimePassed = Settings->TimeScale * DeltaTime;

	for (auto& QueryItem : Systems.Query<UWaterComponent>())
	{
		auto* Water = QueryItem.Get<1>();

		Water->Time += TimePassed;
		Water->ApplyToMaterial();
	}
}
