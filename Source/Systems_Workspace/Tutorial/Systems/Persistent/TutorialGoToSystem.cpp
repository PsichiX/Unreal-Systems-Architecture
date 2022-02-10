#include "Systems_Workspace/Tutorial/Systems/Persistent/TutorialGoToSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Tutorial/Components/TutorialSelectedComponent.h"
#include "Systems_Workspace/Tutorial/Components/TutorialTargetComponent.h"

const FName RIGHT_MOUSE_BUTTON = FName(TEXT("RightMouseButton"));

void TutorialGoToSystem(USystemsWorld& Systems)
{
	const auto* PlayerController = Systems.GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	const auto bIsPointing = PlayerController->IsInputKeyDown(RIGHT_MOUSE_BUTTON);
	if (bIsPointing == false)
	{
		return;
	}

	FHitResult Hit;
	if (PlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_GameTraceChannel1, true, Hit))
	{
		Systems.Query<UTutorialTargetComponent, UTutorialSelectedComponent>().ForEach(
			[&](auto& QueryItem)
			{
				auto* Target = QueryItem.Get<1>();

				Target->Position = Hit.ImpactPoint;
			});
	}
}
