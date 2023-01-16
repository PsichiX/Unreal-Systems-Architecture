#include "SystemsSpatialQuery/Public/Systems/SpatialInformationSystem.h"

#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialGraph.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

void USpatialInformationSystem::Run(USystemsWorld& Systems)
{
	const auto* Information = Systems.Resource<USpatialInformation>();
	const auto* Graph = Cast<USpatialGraph>(Systems.ResourceRaw(this->ResourceType.Get()));
	if (IsValid(Information) == false || IsValid(Graph) == false)
	{
		return;
	}

	// TODO: read `Graph` connections and propagate values in `Information`.
}