#include "Systems_Workspace/Boids/Systems/Persistent/BoidsDebugDrawSystem.h"

#include "DrawDebugHelpers.h"
#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/Components/BoidComponent.h"
#include "Systems_Workspace/Boids/Components/VelocityComponent.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"

const float ARROW_LENGTH = 20;
const float THICKNESS = 8;

void BoidsDebugDrawSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunDebugDraw == false)
	{
		return;
	}

	Systems.Query<UVelocityComponent, UBoidComponent>().ForEach(
		[&](const auto& QueryItem)
		{
			const auto* Actor = QueryItem.Get<0>();
			const auto* Velocity = QueryItem.Get<1>();
			const auto From = Actor->GetActorLocation();
			const auto To = From + Velocity->Value;

			DrawDebugDirectionalArrow(
				Systems.GetWorld(), From, To, ARROW_LENGTH, FColor::Red, false, 0, 0, THICKNESS);
		});
}
