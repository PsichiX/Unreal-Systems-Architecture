#include "Systems_Workspace/Boids/Systems/Persistent/BoidsProximityColorSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Boids/Components/BoidComponent.h"
#include "Systems_Workspace/Boids/Components/ColorComponent.h"
#include "Systems_Workspace/Boids/Resources/BoidsSettings.h"
#include "Systems_Workspace/Boids/Resources/BoidsSystemsRunCriteria.h"
#include "Systems_Workspace/Shared/Components/FoodComponent.h"

template <class T>
void ApplyColors(USystemsWorld& Systems, float PerceptionRange)
{
	Systems.Query<UColorComponent, UBoidComponent>().ForEach(
		[&](auto& QueryItem)
		{
			const auto* Actor = QueryItem.Get<0>();
			auto* Color = QueryItem.Get<1>();
			const auto Position = Actor->GetActorLocation();

			const auto NearestDistance =
				Systems.Query<T>()
					.FilterMap<float>(
						[&](const auto& QueryItem)
						{
							const auto* OtherActor = QueryItem.Get<0>();
							const auto OtherPosition = OtherActor->GetActorLocation();
							const auto Distance = FVector::Distance(Position, OtherPosition);

							return Actor != OtherActor && Distance <= PerceptionRange
								? TOptional<float>(Distance)
								: TOptional<float>();
						})
					.ComparedBy([](const auto A, const auto B) { return A < B; });

			if (NearestDistance.IsSet())
			{
				const auto Factor =
					FMath::Clamp(NearestDistance.GetValue() / PerceptionRange, 0.0f, 1.0f);
				const auto Value =
					FLinearColor::LerpUsingHSV(FLinearColor::Yellow, FLinearColor::Blue, Factor);
				Color->ApplyColor(Value);
			}
			else
			{
				Color->ApplyColor(FLinearColor::Blue);
			}
		});
}

void BoidsProximityColorSystem(USystemsWorld& Systems)
{
	const auto* BoidsSystemsRunCriteria = Systems.Resource<UBoidsSystemsRunCriteria>();
	if (IsValid(BoidsSystemsRunCriteria) && BoidsSystemsRunCriteria->bRunProximityColor == false)
	{
		return;
	}

	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (IsValid(BoidsSettings) == false)
	{
		return;
	}

	switch (BoidsSettings->ProximityColorMode)
	{
		case EBoidsProximityColorMode::None:
			Systems.Query<UColorComponent, UBoidComponent>().ForEach(
				[](auto& QueryItem)
				{
					auto* Color = QueryItem.Get<1>();
					Color->ApplyDefaultColor();
				});
			break;

		case EBoidsProximityColorMode::Separation:
			ApplyColors<UBoidComponent>(Systems, BoidsSettings->SeparationPerceptionRange);
			break;

		case EBoidsProximityColorMode::Alignment:
			ApplyColors<UBoidComponent>(Systems, BoidsSettings->AlignmentPerceptionRange);
			break;

		case EBoidsProximityColorMode::Cohesion:
			ApplyColors<UBoidComponent>(Systems, BoidsSettings->CohesionPerceptionRange);
			break;

		case EBoidsProximityColorMode::Hunt:
			ApplyColors<UFoodComponent>(Systems, BoidsSettings->HuntPerceptionRange);
			break;
	}
}
