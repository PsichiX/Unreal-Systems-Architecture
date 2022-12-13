#include "Life.h"

#include "Systems/Public/SystemsReflection.h"

#include "Life/Systems/Persistent/AgingSystem.h"
#include "Life/Systems/Persistent/AiSystem.h"
#include "Life/Systems/Persistent/ApplyAgeToSpriteSystem.h"
#include "Life/Systems/Persistent/ApplyBlendSpaceFlipbookSystem.h"
#include "Life/Systems/Persistent/ApplyCursorToWorldSystem.h"
#include "Life/Systems/Persistent/CameraRelationSystem.h"
#include "Life/Systems/Persistent/ClearVisionSystem.h"
#include "Life/Systems/Persistent/DebugFollowPathSystem.h"
#include "Life/Systems/Persistent/FaceCameraSystem.h"
#include "Life/Systems/Persistent/FlatMovementSystem.h"
#include "Life/Systems/Persistent/FollowPathSystem.h"
#include "Life/Systems/Persistent/HungerSystem.h"
#include "Life/Systems/Persistent/MoveCameraSystem.h"
#include "Life/Systems/Persistent/RotateCameraSystem.h"
#include "Life/Systems/Persistent/SpawnSystem.h"
#include "Life/Systems/Persistent/WaterSystem.h"
#include "Life/Systems/Persistent/ZoomCameraSystem.h"

#define LOCTEXT_NAMESPACE "FLifeModule"
#define SYSTEMS_NAMESPACE "Life"

void FLifeModule::StartupModule()
{
	REGISTER_SYSTEM_FUNCTION(AgingSystem);
	REGISTER_SYSTEM_FUNCTION(AiSystem);
	REGISTER_SYSTEM_FUNCTION(ApplyAgeToSpriteSystem);
	REGISTER_SYSTEM_FUNCTION(ApplyBlendSpaceFlipbookSystem);
	REGISTER_SYSTEM_FUNCTION(ApplyCursorToWorldSystem);
	REGISTER_SYSTEM_FUNCTION(CameraRelationSystem);
	REGISTER_SYSTEM_FUNCTION(ClearVisionSystem);
	REGISTER_SYSTEM_FUNCTION(DebugFollowPathSystem);
	REGISTER_SYSTEM_FUNCTION(FaceCameraSystem);
	REGISTER_SYSTEM_FUNCTION(FlatMovementSystem);
	REGISTER_SYSTEM_FUNCTION(FollowPathSystem);
	REGISTER_SYSTEM_FUNCTION(HungerSystem);
	REGISTER_SYSTEM_FUNCTION(MoveCameraSystem);
	REGISTER_SYSTEM_FUNCTION(RotateCameraSystem);
	REGISTER_SYSTEM_FUNCTION(SpawnSystem);
	REGISTER_SYSTEM_FUNCTION(WaterSystem);
	REGISTER_SYSTEM_FUNCTION(ZoomCameraSystem);
}

void FLifeModule::ShutdownModule()
{
	UNREGISTER_SYSTEM_FUNCTION(AgingSystem);
	UNREGISTER_SYSTEM_FUNCTION(AiSystem);
	UNREGISTER_SYSTEM_FUNCTION(ApplyAgeToSpriteSystem);
	UNREGISTER_SYSTEM_FUNCTION(ApplyBlendSpaceFlipbookSystem);
	UNREGISTER_SYSTEM_FUNCTION(ApplyCursorToWorldSystem);
	UNREGISTER_SYSTEM_FUNCTION(CameraRelationSystem);
	UNREGISTER_SYSTEM_FUNCTION(ClearVisionSystem);
	UNREGISTER_SYSTEM_FUNCTION(DebugFollowPathSystem);
	UNREGISTER_SYSTEM_FUNCTION(FaceCameraSystem);
	UNREGISTER_SYSTEM_FUNCTION(FlatMovementSystem);
	UNREGISTER_SYSTEM_FUNCTION(FollowPathSystem);
	UNREGISTER_SYSTEM_FUNCTION(HungerSystem);
	UNREGISTER_SYSTEM_FUNCTION(MoveCameraSystem);
	UNREGISTER_SYSTEM_FUNCTION(RotateCameraSystem);
	UNREGISTER_SYSTEM_FUNCTION(SpawnSystem);
	UNREGISTER_SYSTEM_FUNCTION(WaterSystem);
	UNREGISTER_SYSTEM_FUNCTION(ZoomCameraSystem);
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_GAME_MODULE(FLifeModule, Life);
