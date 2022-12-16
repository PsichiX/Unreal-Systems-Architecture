#include "SystemsSpatialQuery/Public/SpatialPartitioningBlueprintLibrary.h"

#include "Systems/Public/SystemsWorld.h"

TArray<AActor*> USpatialPartitioningBlueprintLibrary::GetActorsInArea(USpatialPartitioning* Target, const FArea& Area)
{
	TSet<TObjectPtr<AActor>> Result = {};
	if (IsValid(Target))
	{
		Target->GetActorsInArea(Result, Area);
	}
	return Result.Array();
}

TArray<AActor*> USpatialPartitioningBlueprintLibrary::GetActorsInRadius(USpatialPartitioning* Target,
	FVector Center,
	float Radius)
{
	TSet<TObjectPtr<AActor>> Result = {};
	if (IsValid(Target))
	{
		Target->GetActorsInRadius(Result, Center, Radius);
	}
	return Result.Array();
}

AActor* USpatialPartitioningBlueprintLibrary::FindClosestActor(USpatialPartitioning* Target,
	FVector Position,
	const TArray<AActor*>& Ignored,
	ESpatialPartitioningStatusBranch& Branches)
{
	Branches = ESpatialPartitioningStatusBranch::Failure;
	if (IsValid(Target))
	{
		auto Found =
			Target->FindClosestActor(Position, [&](const auto* Actor) { return Ignored.Contains(Actor) == false; });
		if (Found)
		{
			Branches = ESpatialPartitioningStatusBranch::Success;
			return Found;
		}
	}
	return nullptr;
}
