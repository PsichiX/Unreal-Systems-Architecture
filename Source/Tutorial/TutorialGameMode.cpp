#include "Tutorial/TutorialGameMode.h"

#include "Tutorial/Components/TutorialHighlightComponent.h"
#include "Tutorial/Components/TutorialMovementComponent.h"
#include "Tutorial/Components/TutorialNpcComponent.h"
#include "Tutorial/Components/TutorialSelectedComponent.h"
#include "Tutorial/Components/TutorialTargetComponent.h"
#include "Tutorial/Systems/Persistent/TutorialGoToSystem.h"
#include "Tutorial/Systems/Persistent/TutorialMoveTowardsTargetSystem.h"
#include "Tutorial/Systems/Persistent/TutorialMovementSystem.h"
#include "Tutorial/Systems/Persistent/TutorialSelectActorsSystem.h"

void ATutorialGameMode::BeginPlay()
{
	Super::BeginPlay();

	auto* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
	}
}
