#include "Systems_Workspace/Boids/Systems/Persistent/BoidsPopulationControlSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/Components/BoidComponent.h"
#include "Systems_Workspace/Boids/Components/VelocityComponent.h"
#include "Systems_Workspace/Boids/Resources/BoidsSettings.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Systems_Workspace/Shared/Components/SpaceBoundsComponent.h"

void BoidsPopulationControlSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunPopulationControl == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false || IsValid(BoidsSettings->BoidClass) == false)
	{
		return;
	}

	const auto MinVelocity = BoidsSettings->MinVelocity;
	const auto MaxVelocity = BoidsSettings->MaxVelocity;
	const auto PopulationNumber = BoidsSettings->PopulationNumber;
	auto* BoidClass = BoidsSettings->BoidClass;
	const auto Count = Systems.Query<UBoidComponent>().Count();
	const auto Difference = static_cast<int>(Count) - PopulationNumber;

	if (Difference < 0)
	{
		const auto BoxOpt = Systems.Query<USpaceBoundsComponent>()
								.FilterMap<FBoxSphereBounds>(
									[](const auto& QueryItem)
									{
										const auto* SpaceBounds = QueryItem.Get<1>();
										return SpaceBounds->Bounds;
									})
								.Map<FBox>([](const auto& Bounds) { return Bounds.GetBox(); })
								.First();

		const auto Box = BoxOpt.IsSet() ? BoxOpt.GetValue() : FBox(FVector(0), FVector(0));

		IterRange(0, -Difference)
			.ForEach(
				[&](const auto Index)
				{
					const auto Position = FMath::RandPointInBox(Box);
					auto* Actor =
						Systems.GetWorld()->SpawnActor<AActor>(BoidClass, Position, FRotator());

					if (IsValid(Actor))
					{
						auto* Velocity = Actor->FindComponentByClass<UVelocityComponent>();
						if (IsValid(Velocity))
						{
							Velocity->Randomize(MinVelocity, MaxVelocity);
						}
					}
				});
	}
	else if (Difference > 0)
	{
		Systems.Query<UBoidComponent>()
			.Take(static_cast<uint32>(Difference))
			.ForEach(
				[](const auto& QueryItem)
				{
					auto* Actor = QueryItem.Get<0>();

					Actor->Destroy();
				});
	}
}
