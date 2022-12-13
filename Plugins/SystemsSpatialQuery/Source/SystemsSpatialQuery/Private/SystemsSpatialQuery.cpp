#include "SystemsSpatialQuery/Public/SystemsSpatialQuery.h"

#include "Systems/Public/SystemsReflection.h"
#include "SystemsSpatialQuery/Public/Systems/SpatialpartitioningSystem.h"

#define LOCTEXT_NAMESPACE "FSystemsSpatialQueryModule"
#define SYSTEMS_NAMESPACE "SystemsSpatialQuery"

void FSystemsSpatialQueryModule::StartupModule()
{
	REGISTER_SYSTEM_FUNCTION(SpatialPartitioningSystem);
	REGISTER_SYSTEM_FUNCTION(DebugSpatialPartitioningSystem);
}

void FSystemsSpatialQueryModule::ShutdownModule()
{
	UNREGISTER_SYSTEM_FUNCTION(SpatialPartitioningSystem);
	UNREGISTER_SYSTEM_FUNCTION(DebugSpatialPartitioningSystem);
}

#undef LOCTEXT_NAMESPACE
#undef SYSTEMS_NAMESPACE

IMPLEMENT_MODULE(FSystemsSpatialQueryModule, SystemsSpatialQuery)