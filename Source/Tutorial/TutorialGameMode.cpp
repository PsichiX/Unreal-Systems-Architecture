#include "Tutorial/TutorialGameMode.h"

#include "Shared/Components/SpeedComponent.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Tutorial/Components/TutorialHighlightComponent.h"
#include "Tutorial/Components/TutorialMovementComponent.h"
#include "Tutorial/Components/TutorialNpcComponent.h"
#include "Tutorial/Components/TutorialSelectedComponent.h"
#include "Tutorial/Components/TutorialTargetComponent.h"
#include "Tutorial/Systems/Persistent/TutorialGoToSystem.h"
#include "Tutorial/Systems/Persistent/TutorialMoveTowardsTargetSystem.h"
#include "Tutorial/Systems/Persistent/TutorialMovementSystem.h"
#include "Tutorial/Systems/Persistent/TutorialSelectActorsSystem.h"

const FName ATutorialGameMode::SYSTEMS_WORLD = FName(TEXT("Tutorial"));

void ATutorialGameMode::InitGame(const FString& MapName,
	const FString& Options,
	FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(ThisClass::SYSTEMS_WORLD,
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UTutorialNpcComponent>();
				Systems.RegisterComponent<UTutorialMovementComponent>();
				Systems.RegisterComponent<UTutorialTargetComponent>();
				Systems.RegisterComponent<UTutorialSelectedComponent>();
				Systems.RegisterComponent<UTutorialHighlightComponent>();
				Systems.RegisterComponent<USpeedComponent>();

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

void ATutorialGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->ReleaseSystemsWorld(ThisClass::SYSTEMS_WORLD);
	}
}
