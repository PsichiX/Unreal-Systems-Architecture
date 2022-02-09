#include "Systems_Workspace/Life/Systems/Persistent/RotateCameraSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/GodComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"

void RotateCameraSystem(USystemsWorld& Systems)
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
	const auto Speed = Settings->CameraRotationSpeed * DeltaTime;

	if (auto QueryItem = Systems.Query<UGodComponent>().First())
	{
		auto* Actor = QueryItem.GetValue().Get<0>();
		const auto* God = QueryItem.GetValue().Get<1>();
		if (God->bCameraZoomRotateMode == false)
		{
			return;
		}

		const auto Delta = God->CameraRelativeDirection.X;
		auto Rotation = Actor->GetActorRotation();
		Rotation.Yaw += (Delta * Delta) * FMath::Sign(Delta) * Speed;
		Actor->SetActorRotation(Rotation);
	}
}
