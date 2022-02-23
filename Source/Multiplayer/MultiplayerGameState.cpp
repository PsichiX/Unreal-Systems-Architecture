#include "Multiplayer/MultiplayerGameState.h"

#include "Shared/Components/SpeedComponent.h"
#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Multiplayer/Components/MultiPlayerComponent.h"
#include "Multiplayer/Components/MultiplayerHitComponent.h"
#include "Multiplayer/Components/MultiplayerInputComponent.h"
#include "Multiplayer/Components/MultiplayerLocalControlComponent.h"
#include "Multiplayer/Components/MultiplayerRocketComponent.h"
#include "Multiplayer/Components/MultiplayerSpawnRocketComponent.h"
#include "Multiplayer/MultiplayerGameMode.h"
#include "Multiplayer/Resources/MultiPlayerInput.h"
#include "Multiplayer/Systems/Persistent/MultiplayerApplyInputSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerMovementSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerSpawnRocketSystem.h"

void AMultiplayerGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// NOTE:
	// When doing separate pipelines for server and client, make sure game state won't make its
	// pipeline on server - in that case game mode will setup its own pipeline and game state would
	// add its own so we might get duplicated pipelines which we definitely don't want.
	if (GetWorld()->IsServer())
	{
		return;
	}

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->AcquireSystemsWorld(AMultiplayerGameMode::SYSTEMS_WORLD,
			[&](auto& Systems)
			{
				Systems.RegisterComponent<UMultiPlayerComponent>();
				Systems.RegisterComponent<UMultiplayerRocketComponent>();
				Systems.RegisterComponent<UMultiplayerLocalControlComponent>();
				Systems.RegisterComponent<UMultiplayerInputComponent>();
				Systems.RegisterComponent<UMultiplayerSpawnRocketComponent>();
				Systems.RegisterComponent<USpeedComponent>();
				Systems.RegisterComponent<UVelocityComponent>();

				Systems.InstallResource<UMultiPlayerInput>();

				Systems.InstallLambdaSystem(
					MultiplayerClientApplyInputSystem, FInstallSystemOptions("ApplyInput"));
				Systems.InstallLambdaSystem(
					MultiplayerMovementSystem, FInstallSystemOptions("Movement"));
				Systems.InstallLambdaSystem(
					MultiplayerSpawnRocketSystem, FInstallSystemOptions("SpawnRocket"));
			});
	}
}

void AMultiplayerGameState::BeginPlay()
{
	Super::BeginPlay();

	auto* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

void AMultiplayerGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld()->IsServer())
	{
		return;
	}

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->ReleaseSystemsWorld(AMultiplayerGameMode::SYSTEMS_WORLD);
	}
}
