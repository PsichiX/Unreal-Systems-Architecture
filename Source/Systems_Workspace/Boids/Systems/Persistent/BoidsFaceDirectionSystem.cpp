#include "Systems_Workspace/Boids/Systems/Persistent/BoidsFaceDirectionSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/Components/BoidComponent.h"
#include "Systems_Workspace/Boids/Components/VelocityComponent.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"

void BoidsFaceDirectionSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunFaceDirection == false)
	{
		return;
	}

	Systems.Query<UVelocityComponent, UBoidComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			const auto* Velocity = QueryItem.Get<1>();

			if (Velocity->Value.IsNearlyZero() == false)
			{
				Actor->SetActorRotation(Velocity->Value.Rotation());
			}
		});
}
