#include "Boids/Systems/Persistent/BoidsKeepInSpaceBoundsSystem.h"

#include "Shared/Components/SpaceBoundsComponent.h"
#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"

void KeepInBounds(const FBox& Box, FVector& Position, FVector& Velocity, int Channel)
{
	if (Position[Channel] < Box.Min[Channel])
	{
		Position[Channel] = Box.Max[Channel];
	}
	else if (Position[Channel] > Box.Max[Channel])
	{
		Position[Channel] = Box.Min[Channel];
	}
}

void BoidsKeepInSpaceBoundsSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunKeepInSpaceBounds == false)
	{
		return;
	}

	const auto BoxOpt = Systems.Query<USpaceBoundsComponent>()
							.FilterMap<FBoxSphereBounds>(
								[](const auto& QueryItem)
								{
									const auto* SpaceBounds = QueryItem.Get<1>();
									return SpaceBounds->Bounds;
								})
							.Map<FBox>([](const auto& Bounds) { return Bounds.GetBox(); })
							.First();
	if (BoxOpt.IsSet() == false)
	{
		return;
	}
	const auto Box = BoxOpt.GetValue();

	for (auto& QueryItem : Systems.Query<UVelocityComponent, UBoidComponent>())
	{
		auto* Actor = QueryItem.Get<0>();
		auto* Velocity = QueryItem.Get<1>();
		auto Position = Actor->GetActorLocation();

		if (Box.IsInsideOrOn(Position) == false)
		{
			KeepInBounds(Box, Position, Velocity->Value, 0);
			KeepInBounds(Box, Position, Velocity->Value, 1);
			KeepInBounds(Box, Position, Velocity->Value, 2);

			Actor->SetActorLocation(Position);
		}
	}
}
