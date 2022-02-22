#include "Life/Systems/Persistent/FaceCameraSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/CameraRelationComponent.h"
#include "Life/Components/FaceCameraComponent.h"
#include "Life/Components/GodComponent.h"

void FaceCameraSystem(USystemsWorld& Systems)
{
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

	if (const auto QueryItem = Systems.Query<UGodComponent>().First())
	{
		const auto* God = QueryItem.GetValue().Get<1>();
		const auto CameraDirection = God->GetCameraRotation().Vector();
		const auto Rotation = (-CameraDirection).Rotation();

		Systems.Query<UCameraRelationComponent, UFaceCameraComponent>().ForEach(
			[&](auto& QueryItem)
			{
				auto* Actor = QueryItem.Get<0>();
				const auto* CameraRelation = QueryItem.Get<1>();
				auto* FaceCamera = QueryItem.Get<2>();

				FaceCamera->Metronome.Progress(DeltaTime);

				if (CameraRelation->bIsVisible && FaceCamera->Metronome.ConsumeTicked())
				{
					Actor->SetActorRotation(Rotation);
				}
			});
	}
}
