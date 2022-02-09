#include "Systems_Workspace/Life/Systems/Persistent/CameraRelationSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/CameraRelationComponent.h"
#include "Systems_Workspace/Life/Components/GodComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"

void CameraRelationSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto ConeAngle = Settings->VisibilityConeAngleDegrees;

	if (const auto QueryItem = Systems.Query<UGodComponent>().First())
	{
		const auto* God = QueryItem.GetValue().Get<1>();
		const auto CameraPosition = God->GetCameraPosition();
		const auto CameraDirection = God->GetCameraRotation().Vector().GetSafeNormal();
		const auto Rotation = (-CameraDirection).Rotation();
		const auto Threshold = FMath::Cos(FMath::DegreesToRadians(ConeAngle));

		Systems.Query<UCameraRelationComponent>().ForEach(
			[&](auto& QueryItem)
			{
				const auto* Actor = QueryItem.Get<0>();
				auto* CameraRelation = QueryItem.Get<1>();
				const auto Position = Actor->GetActorLocation();
				const auto Diff = Position - CameraPosition;
				const auto Dot = FVector::DotProduct(CameraDirection, Diff.GetSafeNormal());

				CameraRelation->bIsVisible = Dot >= Threshold;
				CameraRelation->Distance = Diff.Size();
				CameraRelation->Difference = Diff;
			});
	}
}
