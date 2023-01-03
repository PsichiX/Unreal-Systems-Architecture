#include "Tutorial/Systems/Persistent/TutorialMovementSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Tutorial/Components/TutorialMovementComponent.h"

void TutorialMovementSystem(USystemsWorld& Systems)
{
	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();

	for (auto& QueryItem : Systems.Query<UTutorialMovementComponent>())
	{
		auto* Actor = QueryItem.Get<0>();
		auto* Movement = QueryItem.Get<1>();
		const auto Position = Actor->GetActorLocation();
		const auto Velocity = FVector(Movement->Value, 0) * DeltaTime;

		Actor->SetActorLocation(Position + Velocity);

		Movement->Value = FVector2D(0);
	}
}
