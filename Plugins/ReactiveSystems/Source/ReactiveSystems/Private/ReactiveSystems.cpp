#include "ReactiveSystems/Public/ReactiveSystems.h"

#include "ReactiveSystems/Public/Systems/SystemsChangeDetectionSystem.h"
#include "Systems/Public/SystemsReflection.h"

#define LOCTEXT_NAMESPACE "FReactiveSystemsModule"
#define SYSTEMS_NAMESPACE "ReactiveSystems"

void FReactiveSystemsModule::StartupModule()
{
	REGISTER_SYSTEM_FUNCTION(SystemsChangeDetectionSystem);
}

void FReactiveSystemsModule::ShutdownModule()
{
	UNREGISTER_SYSTEM_FUNCTION(SystemsChangeDetectionSystem);
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_MODULE(FReactiveSystemsModule, ReactiveSystems)