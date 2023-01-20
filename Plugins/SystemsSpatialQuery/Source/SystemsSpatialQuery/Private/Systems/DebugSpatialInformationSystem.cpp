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
				auto Color = Found->Color;
				if (Size < 0.0)
				{
					Color.R = 255 - Color.R;
					Color.G = 255 - Color.G;
					Color.B = 255 - Color.B;
				}
				DrawDebugSphere(GetWorld(), Center, Size, this->SphereSegments, Color);
			}
		}
	}
}