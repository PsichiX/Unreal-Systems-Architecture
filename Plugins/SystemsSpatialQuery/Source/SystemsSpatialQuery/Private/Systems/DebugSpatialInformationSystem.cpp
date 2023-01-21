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
				if (Found->DrawMode == EDebugSpatialInformationDrawMode::Scale)
				{
					const auto Size = ValuePair.Value.Value * Found->Scale;
					auto Color = Found->Color;
					if (Size < 0.0)
					{
						Color.R = static_cast<uint8>((static_cast<uint16>(Color.R) + 128) % 256);
						Color.G = static_cast<uint8>((static_cast<uint16>(Color.G) + 128) % 256);
						Color.B = static_cast<uint8>((static_cast<uint16>(Color.B) + 128) % 256);
					}
					DrawDebugSphere(GetWorld(), Center, Size, this->SphereSegments, Color);
				}
				else if (Found->DrawMode == EDebugSpatialInformationDrawMode::Color)
				{
					const auto Factor = FMath::Clamp(ValuePair.Value.Value / Found->ValueRangeLimit, -1.0, 1.0);
					const auto Color = Factor >= 0.0 ? FMath::Lerp(this->NeutralColor, this->PositiveColor, Factor)
													 : FMath::Lerp(this->NeutralColor, this->NegativeColor, -Factor);
					DrawDebugSphere(GetWorld(), Center, Found->Size, this->SphereSegments, Color.ToFColorSRGB());
				}
			}
		}
	}
}