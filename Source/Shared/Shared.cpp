#include "Shared.h"

#include "TestIterators.h"

#define LOCTEXT_NAMESPACE "FSharedModule"
#define SYSTEMS_NAMESPACE "Shared"

void FSharedModule::StartupModule()
{
	TestIterators();
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE(FSharedModule, Shared, "Systems_Workspace");
