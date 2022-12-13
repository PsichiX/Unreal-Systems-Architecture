#include "ReactiveUserInterfaceSystems/Public/ReactiveUserInterfaceSystems.h"

#include "ReactiveUserInterfaceSystems/Public/Systems/UiChangeDetectionSystem.h"
#include "Systems/Public/SystemsReflection.h"

#define LOCTEXT_NAMESPACE "FReactiveUserInterfaceSystemsModule"
#define SYSTEMS_NAMESPACE "ReactiveUserInterfaceSystems"

void FReactiveUserInterfaceSystemsModule::StartupModule()
{
	REGISTER_SYSTEM_FUNCTION(UiChangeDetectionSystem);
}

void FReactiveUserInterfaceSystemsModule::ShutdownModule()
{
	UNREGISTER_SYSTEM_FUNCTION(UiChangeDetectionSystem);
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_MODULE(FReactiveUserInterfaceSystemsModule,
	ReactiveUserInterfaceSystems)