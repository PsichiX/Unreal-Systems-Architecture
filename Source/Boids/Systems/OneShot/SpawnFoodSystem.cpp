#include "Boids/Systems/OneShot/SpawnFoodSystem.h"

#include "Systems/Public/SystemsWorld.h"

#include "Boids/Resources/BoidsSettings.h"
#include "Shared/Components/SpaceBoundsComponent.h"

void SpawnFoodSystem(USystemsWorld& Systems, uint32 Amount)
{
	const auto* BoidsSettings = Systems.Resource<UBoidsSettings>();
	if (Amount <= 0 || IsValid(BoidsSettings) == false ||
		IsValid(BoidsSettings->FoodClass) == false)
	{
		return;
	}

	auto* FoodClass = BoidsSettings->FoodClass;
	if (IsValid(FoodClass) == false)
	{
		return;
	}

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

	IterRange<uint32>(0, Amount).ForEach(
		[&](const auto Index)
		{
			const auto Position = FMath::RandPointInBox(Box);
			Systems.GetWorld()->SpawnActor<AActor>(FoodClass, Position, FRotator());
		});
}
