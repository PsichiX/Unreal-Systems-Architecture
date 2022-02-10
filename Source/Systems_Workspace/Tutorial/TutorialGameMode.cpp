#include "Systems_Workspace/Tutorial/TutorialGameMode.h"

#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Tutorial/Components/TutorialHighlightComponent.h"
#include "Systems_Workspace/Tutorial/Components/TutorialMovementComponent.h"
#include "Systems_Workspace/Tutorial/Components/TutorialNpcComponent.h"
#include "Systems_Workspace/Tutorial/Components/TutorialSelectedComponent.h"
#include "Systems_Workspace/Tutorial/Components/TutorialSpeedComponent.h"
#include "Systems_Workspace/Tutorial/Components/TutorialTargetComponent.h"
#include "Systems_Workspace/Tutorial/Systems/Persistent/TutorialGoToSystem.h"
#include "Systems_Workspace/Tutorial/Systems/Persistent/TutorialMoveTowardsTargetSystem.h"
#include "Systems_Workspace/Tutorial/Systems/Persistent/TutorialMovementSystem.h"
#include "Systems_Workspace/Tutorial/Systems/Persistent/TutorialSelectActorsSystem.h"

const FName ATutorialGameMode::SYSTEMS_WORLD = FName(TEXT("Tutorial"));

void ATutorialGameMode::InitGame(const FString& MapName,
	const FString& Options,
	FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(SYSTEMS_WORLD,
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UTutorialNpcComponent>();
				Systems.RegisterComponent<UTutorialMovementComponent>();
				Systems.RegisterComponent<UTutorialTargetComponent>();
				Systems.RegisterComponent<UTutorialSelectedComponent>();
				Systems.RegisterComponent<UTutorialHighlightComponent>();
				Systems.RegisterComponent<UTutorialSpeedComponent>();

				Systems.InstallLambdaSystem(
					TutorialMoveTowardsTargetSystem, FInstallSystemOptions("MoveTowardsTarget"));
				Systems.InstallLambdaSystem(
					TutorialMovementSystem, FInstallSystemOptions("Movement"));
				Systems.InstallLambdaSystem(
					TutorialSelectActorsSystem, FInstallSystemOptions("SelectActors"));
				Systems.InstallLambdaSystem(TutorialGoToSystem, FInstallSystemOptions("GoTo"));
			});
	}
}

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
