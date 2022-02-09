#include "Systems_Workspace/Life/Systems/OneShot/HumansClearVisionSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/ClearRangeComponent.h"
#include "Systems_Workspace/Life/Components/ClearRangeEmitterComponent.h"
#include "Systems_Workspace/Life/Components/HumanComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"

void HumansClearVisionSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	Systems.Query<UClearRangeComponent, UHumanComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();

			auto* Emitter = Actor->FindComponentByClass<UClearRangeEmitterComponent>();
			if (IsValid(Emitter))
			{
				Emitter->SetRegistered(Settings->bHumansClearVision);
			}
		});
}
