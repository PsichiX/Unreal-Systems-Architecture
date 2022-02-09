#include "Systems_Workspace/Boids/Systems/Persistent/BoidsLimitVelocitySystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/Components/BoidComponent.h"
#include "Systems_Workspace/Boids/Components/VelocityComponent.h"
#include "Systems_Workspace/Boids/Resources/BoidsSettings.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"

void BoidsLimitVelocitySystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunLimitVelocity == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}
	const auto MinVelocity = BoidsSettings->MinVelocity;
	const auto MaxVelocity = BoidsSettings->MaxVelocity;

	Systems.Query<UVelocityComponent, UBoidComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Velocity = QueryItem.Get<1>();
			const auto Length = Velocity->Value.Size();

			if (Velocity->Value.IsNearlyZero())
			{
				Velocity->Randomize(MinVelocity, MaxVelocity);
			}
			if (Length < MinVelocity)
			{
				Velocity->Value = Velocity->Value.GetSafeNormal() * MinVelocity;
			}
			if (Length > MaxVelocity)
			{
				Velocity->Value = Velocity->Value.GetSafeNormal() * MaxVelocity;
			}
		});
}
