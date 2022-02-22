#include "Life/Systems/Persistent/ApplyAgeToSpriteSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/Components/AgeComponent.h"
#include "Life/Components/CameraRelationComponent.h"
#include "Life/Components/FlipbookProxyComponent.h"
#include "Life/Components/SpriteProxyComponent.h"
#include "Life/Resources/LifeSettings.h"

template <class T>
void PerformApplyAgeToSpriteQuery(USystemsWorld& Systems, float TimePassed)
{
	Systems.Query<UAgeComponent, T, UCameraRelationComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Age = QueryItem.Get<1>();
			auto* Sprite = QueryItem.Get<2>();
			const auto* CameraRelation = QueryItem.Get<3>();

			Age->ApplyToSpriteMetronome.Progress(TimePassed);

			if (CameraRelation->bIsVisible && Age->ApplyToSpriteMetronome.ConsumeTicked())
			{
				const auto Scale = Age->ScaleMapping.GetRichCurveConst()->Eval(Age->Value);
				const auto Saturation =
					Age->SaturationMapping.GetRichCurveConst()->Eval(Age->Value);
				Sprite->SetRelativeScale3D(FVector(Scale));
				Sprite->ApplySaturation(Saturation);
			}
		});
}

void ApplyAgeToSpriteSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto TimePassed = Settings->TimeScale * DeltaTime;

	PerformApplyAgeToSpriteQuery<USpriteProxyComponent>(Systems, TimePassed);
	PerformApplyAgeToSpriteQuery<UFlipbookProxyComponent>(Systems, TimePassed);
}
