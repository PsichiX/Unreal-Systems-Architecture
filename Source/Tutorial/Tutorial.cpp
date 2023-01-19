#include "Tutorial.h"

#include "Systems/Public/SystemsReflection.h"

#include "Tutorial/Systems/Persistent/TutorialGoToSystem.h"
#include "Tutorial/Systems/Persistent/TutorialMoveTowardsTargetSystem.h"
#include "Tutorial/Systems/Persistent/TutorialMovementSystem.h"
#include "Tutorial/Systems/Persistent/TutorialSelectActorsSystem.h"

#define LOCTEXT_NAMESPACE "FTutorialModule"
#define SYSTEMS_NAMESPACE "Tutorial"

void FTutorialModule::StartupModule()
{
	REGISTER_SYSTEM_FUNCTION(TutorialGoToSystem);
	REGISTER_SYSTEM_FUNCTION(TutorialMoveTowardsTargetSystem);
	REGISTER_SYSTEM_FUNCTION(TutorialMovementSystem);
	REGISTER_SYSTEM_FUNCTION(TutorialSelectActorsSystem);
}

void FTutorialModule::ShutdownModule()
{
	UNREGISTER_SYSTEM_FUNCTION(TutorialGoToSystem);
	UNREGISTER_SYSTEM_FUNCTION(TutorialMoveTowardsTargetSystem);
	UNREGISTER_SYSTEM_FUNCTION(TutorialMovementSystem);
	UNREGISTER_SYSTEM_FUNCTION(TutorialSelectActorsSystem);
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_GAME_MODULE(FTutorialModule, Tutorial);