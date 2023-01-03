#include "Life/Systems/OneShot/HumansClearVisionSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/ClearRangeComponent.h"
#include "Life/Components/ClearRangeEmitterComponent.h"
#include "Life/Components/HumanComponent.h"
#include "Life/Resources/LifeSettings.h"

void HumansClearVisionSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	for (auto& QueryItem : Systems.Query<UClearRangeComponent, UHumanComponent>())
	{
		auto* Actor = QueryItem.Get<0>();
		auto* Emitter = Actor->FindComponentByClass<UClearRangeEmitterComponent>();
		if (IsValid(Emitter))
		{
			Emitter->SetRegistered(Settings->bHumansClearVision);
		}
	}
}
