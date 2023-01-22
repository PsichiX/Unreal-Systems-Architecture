#include "SystemsSpatialQuery/Public/Systems/DebugSpatialInformationSystem.h"

#include "DrawDebugHelpers.h"
#include "Systems/Public/SystemsWorld.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialDiscretization.h"
#include "SystemsSpatialQuery/Public/Resources/SpatialInformation.h"

FColor ShiftHue(FColor Color)
{
	Color.R = static_cast<uint8>((static_cast<uint16>(Color.G) + 64) % 256);
	Color.G = static_cast<uint8>((static_cast<uint16>(Color.B) + 64) % 256);
	Color.B = static_cast<uint8>((static_cast<uint16>(Color.R) + 64) % 256);
	return Color;
}

void UDebugSpatialInformationSystem::Run(USystemsWorld& Systems)
{
	if (IsValid(this->InformationResourceType.Get()) == false)
	{
		return;
	}
	const auto* Information = Cast<USpatialInformation>(Systems.ResourceRaw(this->InformationResourceType.Get()));
	if (IsValid(Information) == false)
	{
		return;
	}
	for (const auto& PointPair : Information->Points)
	{
		for (const auto& ValuePair : PointPair.Value.Values)
		{
			const auto* Found = this->InformationData.Find(ValuePair.Key);
			if (Found != nullptr)
			{
				const auto Center = PointPair.Key->GetActorLocation();
				if (Found->DrawMode == EDebugSpatialInformationDrawMode::Scale)
				{
					const auto Size = ValuePair.Value.Value * Found->Scale;
					auto Color = Found->Color;
					if (Size < 0.0)
					{
						Color = ShiftHue(Color);
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

	if (IsValid(this->DiscretizationResourceType.Get()) == false)
	{
		return;
	}
	const auto* Discretization =
		Cast<USpatialDiscretization>(Systems.ResourceRaw(this->DiscretizationResourceType.Get()));
	if (IsValid(Discretization) == false)
	{
		return;
	}
	TArray Vertices = {FVector(0.0), FVector(0.0), FVector(0.0)};
	const TArray Indices = {0, 1, 2};
	for (const auto& DataPair : this->InformationData)
	{
		if (DataPair.Value.DrawMode == EDebugSpatialInformationDrawMode::HeightField)
		{
			for (const auto& Triangle : Discretization->TrianglesIter())
			{
				const auto OffsetA = FVector(0.0, 0.0, Information->GetOrDefault(Triangle.A, DataPair.Key));
				const auto OffsetB = FVector(0.0, 0.0, Information->GetOrDefault(Triangle.B, DataPair.Key));
				const auto OffsetC = FVector(0.0, 0.0, Information->GetOrDefault(Triangle.C, DataPair.Key));
				Vertices[0] = Triangle.A->GetActorLocation() + OffsetA * DataPair.Value.Scale;
				Vertices[1] = Triangle.B->GetActorLocation() + OffsetB * DataPair.Value.Scale;
				Vertices[2] = Triangle.C->GetActorLocation() + OffsetC * DataPair.Value.Scale;
				DrawDebugMesh(GetWorld(), Vertices, Indices, DataPair.Value.Color);
				Vertices[0].Z += 5.0;
				Vertices[1].Z += 5.0;
				Vertices[2].Z += 5.0;
				const auto Color = ShiftHue(DataPair.Value.Color);
				DrawDebugLine(GetWorld(), Vertices[0], Vertices[1], Color, false, -1, 1, 5);
				DrawDebugLine(GetWorld(), Vertices[1], Vertices[2], Color, false, -1, 1, 5);
				DrawDebugLine(GetWorld(), Vertices[2], Vertices[0], Color, false, -1, 1, 5);
			}
		}
	}
}