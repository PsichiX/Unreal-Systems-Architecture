#include "Boids/Systems/Persistent/BoidsMovementSystem.h"

#include "Shared/Components/VelocityComponent.h"
#include "Systems/Public/SystemsWorld.h"

#include "Boids/Components/BoidComponent.h"
#include "Boids/Resources/BoidsSettings.h"
#include "Boids/Resources/BoidsSystemsRunCriteria.h"

void UBoidsMovementSystem::Init(USystemsWorld& Systems)
{
	Super::Init(Systems);

	auto* GameEvents = Systems.Resource<UGameEvents>();
	if (IsValid(GameEvents))
	{
		this->MovementStep = GameEvents->MovementStep.Receiver(1);
	}
}

void UBoidsMovementSystem::Run(USystemsWorld& Systems)
{
	Super::Run(Systems);

	// NOTE: usually this is how you receive values from channel:
	// auto ForcedStep = false;
	// while (const auto MessageOpt = this->MovementStep.Receive())
	// {
	// 	ForcedStep = true;
	// }
	// but since all we care about is any notification, we can reduce to this:
	const auto ForcedStep = this->MovementStep.IsEmpty() == false;
	this->MovementStep.Clear();

	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (ForcedStep == false && IsValid(BoidsSystemsRunCriteria) &&
		BoidsSystemsRunCriteria->bRunMovement == false)
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

	for (auto& QueryItem : Systems.Query<UVelocityComponent, UBoidComponent>())
	{
		auto* Actor = QueryItem.Get<0>();
		const auto* Velocity = QueryItem.Get<1>();
		const auto Position = Actor->GetActorLocation() + Velocity->Value * DeltaTime;

		Actor->SetActorLocation(Position);
	}
}