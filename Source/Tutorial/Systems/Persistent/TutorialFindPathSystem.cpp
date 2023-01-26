#include "Tutorial/Systems/Persistent/TutorialFindPathSystem.h"

#include "Resources/SpatialGraph.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Components/SpatialComponent.h"

#include "Tutorial/Components/TutorialSelectedComponent.h"
#include "Tutorial/Resources/TutorialPath.h"
#include "Tutorial/Systems/OneShot/TutorialApplyPathToHighlightSystem.h"

const FName MIDDLE_MOUSE_BUTTON = FName(TEXT("MiddleMouseButton"));

void TutorialFindPathSystem(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	const auto bIsSelecting = PlayerController->IsInputKeyDown(MIDDLE_MOUSE_BUTTON);
	if (bIsSelecting == false)
	{
		return;
	}

	auto* Path = Systems.Resource<UTutorialPath>();
	const auto* Graph = Systems.Resource<USpatialGraph>();
	if (IsValid(Path) == false || IsValid(Graph) == false)
	{
		return;
	}

	FHitResult Hit;
	if (PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel2, true, Hit))
	{
		auto* Actor = Hit.GetActor();
		if (IsValid(Actor))
		{
			auto* Spatial = Actor->FindComponentByClass<USpatialComponent>();
			if (IsValid(Spatial))
			{
				for (const auto& QueryItem : Systems.Query<UTutorialSelectedComponent>().Take(1))
				{
					Path->Path =
						Graph->FindPath(QueryItem.Get<0>(), Actor, DistanceSpatialGraphHeuristics)
							.Consume();
				}
			}
		}
	}
	else
	{
		Path->Path.Reset();
	}

	TutorialApplyPathToHighlightSystem(Systems);
}