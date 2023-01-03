#include "Life/Systems/Persistent/CameraRelationSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/CameraRelationComponent.h"
#include "Life/Components/GodComponent.h"
#include "Life/Resources/LifeSettings.h"

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

		for (auto& QueryItem2 : Systems.Query<UCameraRelationComponent>())
		{
			const auto* Actor = QueryItem2.Get<0>();
			auto* CameraRelation = QueryItem2.Get<1>();
			const auto Position = Actor->GetActorLocation();
			const auto Diff = Position - CameraPosition;
			const auto Dot = FVector::DotProduct(CameraDirection, Diff.GetSafeNormal());

			CameraRelation->bIsVisible = Dot >= Threshold;
			CameraRelation->Distance = Diff.Size();
			CameraRelation->Difference = Diff;
		}
	}
}
