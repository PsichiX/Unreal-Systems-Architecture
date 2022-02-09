#include "Systems_Workspace/Life/Systems/Persistent/ZoomCameraSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/GodComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"

void ZoomCameraSystem(USystemsWorld& Systems)
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

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto Speed = Settings->CameraZoomSpeed * DeltaTime;

	if (auto QueryItem = Systems.Query<UGodComponent>().First())
	{
		auto* God = QueryItem.GetValue().Get<1>();
		if (God->bCameraZoomRotateMode == false)
		{
			return;
		}

		const auto Delta = God->CameraRelativeDirection.Y;
		const auto DeltaZoom = (Delta * Delta) * FMath::Sign(Delta) * Speed;
		God->SetCameraZoom(God->GetCameraZoom() + DeltaZoom);
	}
}
