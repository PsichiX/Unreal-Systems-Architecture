#include "Systems_Workspace/Tutorial/Systems/Persistent/TutorialSelectActorsSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Tutorial/Components/TutorialNpcComponent.h"
#include "Systems_Workspace/Tutorial/Components/TutorialSelectedComponent.h"
#include "Systems_Workspace/Tutorial/Systems/OneShot/TutorialApplySelectionToHighlightSystem.h"

const FName LEFT_MOUSE_BUTTON = FName(TEXT("LeftMouseButton"));
const FName LEFT_SHIFT_BUTTON = FName(TEXT("LeftShift"));
const FName LEFT_ALT_BUTTON = FName(TEXT("LeftAlt"));

void TutorialSelectActorsSystem(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	const auto bIsSelecting = PlayerController->IsInputKeyDown(LEFT_MOUSE_BUTTON);
	if (bIsSelecting == false)
	{
		return;
	}

	const auto bAddition = PlayerController->IsInputKeyDown(LEFT_SHIFT_BUTTON);
	const auto bSubtraction = PlayerController->IsInputKeyDown(LEFT_ALT_BUTTON);

	FHitResult Hit;
	if (PlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_GameTraceChannel2, true, Hit))
	{
		auto* Actor = Hit.Actor.Get();
		if (IsValid(Actor))
		{
			auto* Selected = Actor->FindComponentByClass<UTutorialSelectedComponent>();
			if (IsValid(Selected))
			{
				if (bAddition)
				{
					Selected->SetRegistered(true);
				}
				else if (bSubtraction)
				{
					Selected->SetRegistered(false);
				}
				else
				{
					Systems.Query<UTutorialSelectedComponent>().ForEach(
						[](auto& QueryItem)
						{
							auto* Selected = QueryItem.Get<1>();

							Selected->SetRegistered(false);
						});

					Selected->SetRegistered(true);
				}
			}
		}
	}
	else
	{
		Systems.Query<UTutorialSelectedComponent>().ForEach(
			[](auto& QueryItem)
			{
				auto* Selected = QueryItem.Get<1>();

				Selected->SetRegistered(false);
			});
	}

	TutorialApplySelectionToHighlightSystem(Systems);
}
