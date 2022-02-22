#include "Life/AI/Perceptions/UtilityAiFindNearestActorPerception.h"

#include "Systems/Public/Iterator.h"

#include "Life/AI/Reasoner/UtilityAiMemory.h"

struct Meta
{
	float DistanceSquared = 0;
	AActor* Actor = nullptr;
};

void UUtilityAiFindNearestActorPerception::Perceive(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory) const
{
	const auto Position = Actor->GetActorLocation();

	if (auto* ActorsList = Memory.Access(this->SourceMemoryProperty).AsArray())
	{
		const auto Found = IterStd(*ActorsList)
							   .FilterMap<Meta>(
								   [&](auto& Value)
								   {
									   auto* OtherActor = Value.CastObject<AActor>();
									   if (IsValid(OtherActor))
									   {
										   const auto OtherPosition =
											   OtherActor->GetActorLocation();
										   const auto DistanceSquared =
											   FVector::DistSquared(Position, OtherPosition);

										   return TOptional<Meta>({DistanceSquared, OtherActor});
									   }

									   return TOptional<Meta>();
								   })
							   .ComparedBy([](const auto& A, const auto& B)
								   { return A.DistanceSquared < B.DistanceSquared; });

		if (Found.IsSet())
		{
			Memory.Access(this->TargetMemoryProperty) = Found.GetValue().Actor;
		}
	}
}
