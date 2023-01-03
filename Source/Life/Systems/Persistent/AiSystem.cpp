#include "Life/Systems/Persistent/AiSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/AI/Reasoner/UtilityAiReasoner.h"
#include "Life/Components/AiComponent.h"
#include "Life/Components/CameraRelationComponent.h"
#include "Life/Components/GodComponent.h"
#include "Life/Resources/LifeSettings.h"

struct Meta
{
	float Difference = 0;
	float DecideDelay = 0;
};

void AiSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto& LOD = Settings->AiLOD;
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto TimePassed = Settings->TimeScale * DeltaTime;

	for (auto& QueryItem : Systems.Query<UAiComponent, UCameraRelationComponent>())
	{
		const auto* Actor = QueryItem.Get<0>();
		auto* Ai = QueryItem.Get<1>();
		if (IsValid(Ai->Reasoner) == false)
		{
			return;
		}

		const auto* CameraRelation = QueryItem.Get<2>();

		const auto Found = IterStdConst(LOD)
							   .Map<Meta>(
								   [&](const auto& Info)
								   {
									   const auto Difference =
										   FMath::Abs(Info.Distance - CameraRelation->Distance);

									   return CameraRelation->bIsVisible
										   ? Meta{Difference, Info.DecideDelay}
										   : Meta{Difference, Info.DecideOffscreenDelay};
								   })
							   .ComparedBy([](const auto& A, const auto& B)
								   { return A.Difference < B.Difference; });

		if (Found.IsSet())
		{
			Ai->TryDecide(Systems, Found.GetValue().DecideDelay, TimePassed);
		}
	}
}
