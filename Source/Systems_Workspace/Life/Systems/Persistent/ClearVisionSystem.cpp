#include "Systems_Workspace/Life/Systems/Persistent/ClearVisionSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/CameraRelationComponent.h"
#include "Systems_Workspace/Life/Components/ClearRangeComponent.h"
#include "Systems_Workspace/Life/Components/ClearRangeEmitterComponent.h"
#include "Systems_Workspace/Life/Components/ClearRangeReceiverComponent.h"
#include "Systems_Workspace/Life/Components/FlipbookProxyComponent.h"
#include "Systems_Workspace/Life/Components/GodComponent.h"
#include "Systems_Workspace/Life/Components/SpriteProxyComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"

// NOTE: First of all, this effect exists only for the sole purpose of showcasing how one could make
// it brute force way, but it definitely would benefit more when using R-Tree to speed up spatial
// queries. Even better if user would move this effect entirely to GPU and render clear emitters
// into either stencil buffer and mask clear receivers.
template <class T>
void PerformClearVisionQuery(USystemsWorld& Systems, float ClearOpacity)
{
	Systems.Query<T, UCameraRelationComponent, UClearRangeReceiverComponent>()
		.Filter(
			[](const auto& QueryItem)
			{
				const auto* CameraRelation = QueryItem.Get<2>();
				return CameraRelation->bIsVisible;
			})
		.ForEach(
			[&](auto& QueryItem)
			{
				const auto* Actor = QueryItem.Get<0>();
				auto* Sprite = QueryItem.Get<1>();
				const auto Position = Actor->GetActorLocation();

				const auto Found =
					Systems
						.Query<UClearRangeComponent,
							UClearRangeEmitterComponent,
							UCameraRelationComponent>()
						.Filter(
							[](const auto& QueryItem)
							{
								const auto* CameraRelation = QueryItem.Get<3>();
								return CameraRelation->bIsVisible;
							})
						.FilterMap<float>(
							[&](const auto& QueryItem)
							{
								const auto* Clear = QueryItem.Get<1>();
								const auto* Emitter = QueryItem.Get<2>();
								const auto OtherPosition = Emitter->GetComponentLocation();
								auto Distance = INFINITY;
								if (Clear->bScreenSpace)
								{
									const auto* CameraRelation = QueryItem.Get<3>();
									FVector Ignore;
									Distance = FMath::PointDistToLine(Position,
										CameraRelation->Difference,
										OtherPosition,
										Ignore);
								}
								else
								{
									Distance = FVector::Distance(Position, OtherPosition);
								}

								if (Distance < Clear->RangeMin)
								{
									return TOptional<float>(0);
								}
								else if (Distance < Clear->RangeMax)
								{
									const auto Factor = FMath::Clamp((Distance - Clear->RangeMin) /
											(Clear->RangeMax - Clear->RangeMin),
										0.0f,
										1.0f);
									return TOptional<float>(
										FMath::Lerp(ClearOpacity, 1.0f, Factor));
								}
								else
								{
									return TOptional<float>();
								}
							})
						.ComparedBy([](const auto A, const auto B) { return A < B; });

				if (Found.IsSet())
				{
					Sprite->ApplyVisibility(Found.GetValue());
				}
				else
				{
					Sprite->ApplyVisibility(1);
				}
			});
}

void ClearVisionSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto ClearOpacity = Settings->CameraClearOpacity;

	PerformClearVisionQuery<USpriteProxyComponent>(Systems, ClearOpacity);
	PerformClearVisionQuery<UFlipbookProxyComponent>(Systems, ClearOpacity);
}
