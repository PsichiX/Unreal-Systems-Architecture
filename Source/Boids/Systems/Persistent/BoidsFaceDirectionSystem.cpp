#include "Boids/Systems/Persistent/BoidsFaceDirectionSystem.h"

#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"

void BoidsFaceDirectionSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunFaceDirection == false)
	{
		return;
	}

	for (auto& QueryItem : Systems.Query<UVelocityComponent, UBoidComponent>())
	{
		auto* Actor = QueryItem.Get<0>();
		const auto* Velocity = QueryItem.Get<1>();

		if (Velocity->Value.IsNearlyZero() == false)
		{
			Actor->SetActorRotation(Velocity->Value.Rotation());
		}
	}
}
