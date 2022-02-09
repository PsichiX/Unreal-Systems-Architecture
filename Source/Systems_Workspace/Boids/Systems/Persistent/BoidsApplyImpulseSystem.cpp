#include "Systems_Workspace/Boids/Systems/Persistent/BoidsApplyImpulseSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/Components/BoidComponent.h"
#include "Systems_Workspace/Boids/Components/ImpulseComponent.h"
#include "Systems_Workspace/Boids/Components/VelocityComponent.h"
#include "Systems_Workspace/Boids/Resources/BoidsSettings.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Systems_Workspace/Shared/Utils.h"

void BoidsApplyImpulseSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunApplyImpulse == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}
	const auto TimeScale = BoidsSettings->TimeScale;
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds() * TimeScale;

	Systems.Query<UImpulseComponent, UVelocityComponent, UBoidComponent>().ForEach(
		[&](const auto& QueryItem)
		{
			const auto* Actor = QueryItem.Get<0>();
			auto* Impulse = QueryItem.Get<1>();
			auto* Velocity = QueryItem.Get<2>();

			Velocity->Value += Impulse->Value * DeltaTime;
			Impulse->Value = FVector(0);
		});
}
