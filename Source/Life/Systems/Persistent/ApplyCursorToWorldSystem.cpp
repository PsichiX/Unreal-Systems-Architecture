#include "Life/Systems/Persistent/ApplyCursorToWorldSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/ClearRangeComponent.h"
#include "Life/Components/ClearRangeEmitterComponent.h"
#include "Life/Components/CursorComponent.h"
#include "Life/Resources/LifeSettings.h"
#include "Life/Resources/LifeSpawner.h"

void SpawnCursorSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings))
	{
		Systems.GetWorld()->SpawnActor<AActor>(Settings->CursorType);
	}
}

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