#include "Multiplayer.h"

#include "Systems/Public/SystemsReflection.h"

#include "Multiplayer/Systems/Persistent/MultiplayerApplyInputSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerCorrectTransformSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerKillActorsSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerMovementSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerRocketsLifetimeSystem.h"
#include "Multiplayer/Systems/Persistent/MultiplayerSpawnRocketSystem.h"

#define LOCTEXT_NAMESPACE "FMultiplayerModule"
#define SYSTEMS_NAMESPACE "Multiplayer"

void FMultiplayerModule::StartupModule()
{
	REGISTER_SYSTEM_FUNCTION(MultiplayerServerApplyInputSystem);
	REGISTER_SYSTEM_FUNCTION(MultiplayerClientApplyInputSystem);
	REGISTER_SYSTEM_FUNCTION(MultiplayerCorrectTransformSystem);
	REGISTER_SYSTEM_FUNCTION(MultiplayerKillActorsSystem);
	REGISTER_SYSTEM_FUNCTION(MultiplayerMovementSystem);
	REGISTER_SYSTEM_FUNCTION(MultiplayerRocketsLifetimeSystem);
	REGISTER_SYSTEM_FUNCTION(MultiplayerSpawnRocketSystem);
	REGISTER_SYSTEM_FUNCTION(MultiplayerSpawnRocketCooldownSystem);
	REGISTER_SYSTEM_FUNCTION(MultiplayerExecuteRocketSpawnsSystem);
}

void FMultiplayerModule::ShutdownModule()
{
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerServerApplyInputSystem);
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerClientApplyInputSystem);
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerCorrectTransformSystem);
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerKillActorsSystem);
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerMovementSystem);
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerRocketsLifetimeSystem);
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerSpawnRocketSystem);
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerSpawnRocketCooldownSystem);
	UNREGISTER_SYSTEM_FUNCTION(MultiplayerExecuteRocketSpawnsSystem);
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_GAME_MODULE(FMultiplayerModule, Multiplayer);
