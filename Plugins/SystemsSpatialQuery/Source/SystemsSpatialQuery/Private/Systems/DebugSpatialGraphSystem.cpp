#include "SystemsSpatialQuery/Public/Systems/DebugSpatialGraphSystem.h"

#include "DrawDebugHelpers.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialGraph.h"

void UDebugSpatialGraphSystem::Run(USystemsWorld& Systems)
{
	if (IsValid(this->ResourceType.Get()) == false)
	{
		return;
	}
	const auto* Graph = Cast<USpatialGraph>(Systems.ResourceRaw(this->ResourceType.Get()));
	if (IsValid(Graph) == false)
	{
		return;
	}
	if (this->bDrawConnections)
	{
		Graph->ConnectionsIter().ForEach(
			[&](const auto& Pair)
			{
				const auto From = Pair.Get<0>()->GetActorLocation();
				const auto To = Pair.Get<1>()->GetActorLocation();
				DrawDebugLine(GetWorld(), From, To, this->ConnectionsColor);
			});
	}
	if (this->bDrawNodes)
	{
		Graph->NodesIter().ForEach(
			[&](const auto& Node)
			{
				FVector Origin = {};
				FVector Extents = {};
				Node->GetActorBounds(false, Origin, Extents, false);
				DrawDebugBox(GetWorld(), Origin, Extents, this->NodesColor);
			});
	}
}
