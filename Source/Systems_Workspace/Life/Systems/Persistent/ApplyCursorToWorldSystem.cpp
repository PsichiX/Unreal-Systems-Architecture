#include "Systems_Workspace/Life/Systems/Persistent/ApplyCursorToWorldSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/ClearRangeComponent.h"
#include "Systems_Workspace/Life/Components/ClearRangeEmitterComponent.h"
#include "Systems_Workspace/Life/Components/CursorComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSpawner.h"

void ApplyCursorToWorldSystem(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	const auto* Spawner = Systems.Resource<ULifeSpawner>();
	if (IsValid(Spawner) == false)
	{
		return;
	}

	if (auto QueryItem = Systems.Query<UClearRangeComponent, UCursorComponent>().First())
	{
		auto* Actor = QueryItem.GetValue().Get<0>();

		FHitResult Hit;
		const auto bClearEnabled = IsValid(Spawner->SelectedToSpawn) &&
			PlayerController->GetHitResultUnderCursor(
				ECollisionChannel::ECC_GameTraceChannel1, true, Hit);

		if (bClearEnabled)
		{
			Actor->SetActorLocation(Hit.ImpactPoint);
		}

		Actor->SetActorHiddenInGame(bClearEnabled == false);

		auto* Emitter = Actor->FindComponentByClass<UClearRangeEmitterComponent>();
		if (IsValid(Emitter))
		{
			Emitter->SetRegistered(bClearEnabled);
		}
	}
}
