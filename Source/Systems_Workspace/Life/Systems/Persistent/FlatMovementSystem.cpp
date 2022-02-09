#include "Systems_Workspace/Life/Systems/Persistent/FlatMovementSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/Components/FlatMovementComponent.h"
#include "Systems_Workspace/Life/Resources/LifeSettings.h"

const float TRACE_RANGE = 10000.0f;

void FlatMovementSystem(USystemsWorld& Systems)
{
	const auto* Settings = Systems.Resource<ULifeSettings>();
	if (IsValid(Settings) == false)
	{
		return;
	}

	const auto DeltaTime = Systems.GetWorld()->GetDeltaSeconds();
	const auto TimePassed = Settings->TimeScale * DeltaTime;

	Systems.Query<UFlatMovementComponent>().ForEach(
		[&](auto& QueryItem)
		{
			auto* Actor = QueryItem.Get<0>();
			auto* Movement = QueryItem.Get<1>();
			auto Position = Actor->GetActorLocation() + FVector(Movement->Value, 0) * TimePassed;

			FHitResult Hit;
			if (Systems.GetWorld()->LineTraceSingleByChannel(Hit,
					Position + FVector(0, 0, TRACE_RANGE),
					Position - FVector(0, 0, TRACE_RANGE),
					ECollisionChannel::ECC_GameTraceChannel1))
			{
				Position.Z = Hit.ImpactPoint.Z;
			}
			Actor->SetActorLocation(Position);

			Movement->Value = FVector2D(0);
		});
}
