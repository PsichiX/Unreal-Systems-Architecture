#include "SystemsSpatialQuery/Public/Systems/DebugSpatialInformationSystem.h"

#include "DrawDebugHelpers.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

void UDebugSpatialInformationSystem::Run(USystemsWorld& Systems)
{
	if (IsValid(this->ResourceType.Get()) == false)
	{
		return;
	}
	const auto* Information = Cast<USpatialInformation>(Systems.ResourceRaw(this->ResourceType.Get()));
	if (IsValid(Information) == false)
	{
		return;
	}
	for (const auto& PointPair : Information->Points)
	{
		for (const auto& ValuePair : PointPair.Value.Values)
		{
			const auto* Found = this->InformationColors.Find(ValuePair.Key);
			if (Found != nullptr)
			{
				const auto Center = PointPair.Key->GetActorLocation();
				const auto Size = ValuePair.Value.Value * Found->Scale;
				DrawDebugSphere(GetWorld(), Center, Size, this->SphereSegments, Found->Color);
			}
		}
	}
}