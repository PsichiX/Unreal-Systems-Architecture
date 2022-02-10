#include "Systems_Workspace/Tutorial/Systems/OneShot/TutorialApplySelectionToHighlightSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Tutorial/Components/TutorialHighlightComponent.h"
#include "Systems_Workspace/Tutorial/Components/TutorialSelectedComponent.h"

void TutorialApplySelectionToHighlightSystem(USystemsWorld& Systems)
{
	Systems.Query<UTutorialHighlightComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			auto* Highlight = QueryItem.Get<1>();
			const auto bIsSelected = IsValid(Systems.Component<UTutorialSelectedComponent>(Actor));

			Highlight->ApplyHighlighted(bIsSelected);
		});
}
