#include "Tutorial/Systems/OneShot/TutorialApplyPathToHighlightSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Tutorial/Components/TutorialHighlightComponent.h"
#include "Tutorial/Resources/TutorialPath.h"

void TutorialApplyPathToHighlightSystem(USystemsWorld& Systems)
{
	const auto* Path = Systems.Resource<UTutorialPath>();
	if (IsValid(Path) == false)
	{
		return;
	}

	for (auto& QueryItem : Systems.Query<UTutorialHighlightComponent>())
	{
		auto* Actor = QueryItem.Get<0>();
		auto* Highlight = QueryItem.Get<1>();
		const auto bIsSelected = Path->Path.Contains(Actor);

		Highlight->ApplyHighlighted(bIsSelected);
	}
}