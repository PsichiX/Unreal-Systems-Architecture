#include "Multiplayer/MultiplayerGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Shared/Components/SpeedComponent.h"
#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsSubsystem.h"
#include "Systems/Public/SystemsWorld.h"

#include "Multiplayer/Components/MultiPlayerComponent.h"
#include "Multiplayer/Components/MultiplayerCorrectTransformComponent.h"
#include "Multiplayer/Components/MultiplayerHitComponent.h"
#include "Multiplayer/Components/MultiplayerInputComponent.h"
#include "Multiplayer/Components/MultiplayerLocalControlComponent.h"
#include "Multiplayer/Components/MultiplayerRocketComponent.h"
#include "Multiplayer/Components/MultiplayerSpawnRocketComponent.h"
#include "Multiplayer/Resources/MultiPlayerInput.h"
#include "Multiplayer/Resources/MultiplayerCommands.h"
#include "Multiplayer/Systems/Persistent/MultiplayerApplyInputSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerCorrectTransformSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerKillActorsSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerMovementSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerRocketsLifetimeSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerSpawnRocketSystem.h"

const FName AMultiplayerGameMode::SYSTEMS_WORLD = FName(TEXT("MultiplayerGameplay"));
const FName AMultiplayerGameMode::LEVEL_NAME = FName(TEXT("MultiplayerGameMap"));

void AMultiplayerGameMode::InitGame(const FString& MapName,
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
				Systems.RegisterComponent<UMultiPlayerComponent>();
				Systems.RegisterComponent<UMultiplayerRocketComponent>();
				Systems.RegisterComponent<UMultiplayerLocalControlComponent>();
				Systems.RegisterComponent<UMultiplayerInputComponent>();
				Systems.RegisterComponent<UMultiplayerSpawnRocketComponent>();
				Systems.RegisterComponent<UMultiplayerCorrectTransformComponent>();
				Systems.RegisterComponent<UMultiplayerHitComponent>();
				Systems.RegisterComponent<USpeedComponent>();
				Systems.RegisterComponent<UVelocityComponent>();

				Systems.InstallResource<UMultiPlayerInput>();
				Systems.InstallResourceRaw(this->MultiplayerCommands);

				Systems.InstallLambdaSystem(
					MultiplayerServerApplyInputSystem, FInstallSystemOptions("ApplyInput"));
				Systems.InstallLambdaSystem(
					MultiplayerMovementSystem, FInstallSystemOptions("Movement"));
				Systems.InstallLambdaSystem(
					MultiplayerCorrectTransformSystem, FInstallSystemOptions("CorrectTransform"));
				Systems.InstallLambdaSystem(MultiplayerSpawnRocketCooldownSystem,
					FInstallSystemOptions("SpawnRocketCooldown"));
				Systems.InstallLambdaSystem(
					MultiplayerSpawnRocketSystem, FInstallSystemOptions("SpawnRocket"));
				Systems.InstallLambdaSystem(
					MultiplayerRocketsLifetimeSystem, FInstallSystemOptions("RocketsLifetime"));
				Systems.InstallLambdaSystem(MultiplayerExecuteRocketSpawnsSystem,
					FInstallSystemOptions("ExecuteRocketSpawns"));
				Systems.InstallLambdaSystem(
					MultiplayerKillActorsSystem, FInstallSystemOptions("KillActors"));
			});
	}

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(), APlayerStart::StaticClass(), this->PlayersSpawnPoints);
}

void AMultiplayerGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto* Subsystem = USystemsSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->ReleaseSystemsWorld(ThisClass::SYSTEMS_WORLD);
	}
}

AActor* AMultiplayerGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (this->PlayersSpawnPoints.Num() > 0)
	{
		return this->PlayersSpawnPoints.Pop(false);
	}
	return nullptr;
}
