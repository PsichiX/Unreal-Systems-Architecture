#include "Life/Systems/Persistent/SpawnSystem.h"

#include "NavigationSystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/ClearRangeEmitterComponent.h"
#include "Life/Components/GodComponent.h"
#include "Life/Resources/LifeSettings.h"
#include "Life/Resources/LifeSpawner.h"

const FVector EXTENTS = FVector(100);

void SpawnSystem(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto* Spawner = Systems.Resource<ULifeSpawner>();
	if (IsValid(Spawner) == false || IsValid(Spawner->SelectedToSpawn) == false)
	{
		return;
	}

	const auto* Navigation = UNavigationSystemV1::GetCurrent(Systems.GetWorld());
	if (IsValid(Navigation) == false)
	{
		return;
	}

	if (auto QueryItem = Systems.Query<UGodComponent>().First())
	{
		auto* God = QueryItem.GetValue().Get<1>();
		if (God->bAction)
		{
			FHitResult Hit;
			if (PlayerController->GetHitResultUnderCursor(
					ECollisionChannel::ECC_GameTraceChannel1, true, Hit))
			{
				FNavLocation Ignore;
				if (Navigation->ProjectPointToNavigation(Hit.ImpactPoint, Ignore, EXTENTS))
				{
					auto* Actor = Systems.GetWorld()->SpawnActor<AActor>(
						Spawner->SelectedToSpawn, Hit.ImpactPoint, FRotator());

					if (IsValid(Actor))
					{
						auto* Emitter = Actor->FindComponentByClass<UClearRangeEmitterComponent>();
						if (IsValid(Emitter))
						{
							Emitter->SetRegistered(Settings->bHumansClearVision);
						}
					}

					// We consume action to not allow later systems in pipeline depending on action
					// being pressed do their job. If we don't do that, other actions might be
					// triggered while we only want to spawn actor.
					God->bAction = false;
				}
			}
		}
	}
}
