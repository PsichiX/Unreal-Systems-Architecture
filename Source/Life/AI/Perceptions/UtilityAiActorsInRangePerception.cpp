#include "Life/AI/Perceptions/UtilityAiActorsInRangePerception.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/AI/Reasoner/UtilityAiMemory.h"

void UUtilityAiActorsInRangePerception::Perceive(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory) const
{
	const auto Position = Actor->GetActorLocation();

	auto Query = Systems.TaggedQuery();
	for (const auto& Type : this->RequiredComponents)
	{
		Query.TagRaw(Type.Get());
	}

	const auto RangeMinSquared = this->RangeMin * this->RangeMin;
	const auto RangeMaxSquared = this->RangeMax * this->RangeMax;

	const auto Found =
		Query.Iter()
			.FilterMap<FUtilityAiMemoryValue>(
				[&](const auto& QueryItem)
				{
					auto* OtherActor = QueryItem.Get<0>();
					const auto OtherPosition = OtherActor->GetActorLocation();
					const auto DistanceSquared = FVector::DistSquared(Position, OtherPosition);

					return DistanceSquared >= RangeMinSquared && DistanceSquared <= RangeMaxSquared
						? TOptional<FUtilityAiMemoryValue>(OtherActor)
						: TOptional<FUtilityAiMemoryValue>();
				})
			.CollectArray();

	Memory.Access(this->TargetMemoryProperty) = Found;
}
