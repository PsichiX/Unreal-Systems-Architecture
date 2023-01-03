#include "Life/Systems/Persistent/ApplyBlendSpaceFlipbookSystem.h"

#include "PaperFlipbook.h"
#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/CameraRelationComponent.h"
#include "Life/Components/FlatMovementComponent.h"
#include "Life/Components/FlipbookBlendSpaceComponent.h"
#include "Life/Components/FlipbookProxyComponent.h"
#include "Life/Resources/LifeSettings.h"

void ApplyBlendSpaceFlipbookSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto TimeScale = Settings->TimeScale;

	for (auto& QueryItem : Systems.Query<UFlipbookProxyComponent,
						   UFlipbookBlendSpaceComponent,
						   UCameraRelationComponent,
						   UFlatMovementComponent>())
	{
		auto* Flipbook = QueryItem.Get<1>();
		const auto* BlendSpace = QueryItem.Get<2>();
		const auto* Relation = QueryItem.Get<3>();
		const auto* Movement = QueryItem.Get<4>();

		if (Relation->bIsVisible)
		{
			const auto RelationDirection = FVector2D(Relation->Difference).GetSafeNormal();
			const auto MovementDirection = Movement->Value.GetSafeNormal();
			const auto Y = FVector2D::DotProduct(RelationDirection, MovementDirection);
			const auto X = FVector2D::CrossProduct(RelationDirection, MovementDirection);
			const auto Direction = FVector2D(X, -Y);
			const auto PlayRate =
				BlendSpace->FindCurrentPlayRateToSpeedFactor() * Movement->Value.Size();
			auto* Found = BlendSpace->FindCurrentFlipbook(Direction);

			if (IsValid(Found) && Flipbook->GetFlipbook() != Found)
			{
				Flipbook->SetFlipbook(Found);
			}
			Flipbook->SetPlayRate(PlayRate * TimeScale);
		}
	}
}
