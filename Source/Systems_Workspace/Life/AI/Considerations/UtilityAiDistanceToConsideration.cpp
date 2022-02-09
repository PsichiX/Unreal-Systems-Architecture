#include "Systems_Workspace/Life/AI/Considerations/UtilityAiDistanceToConsideration.h"

#include "Systems/Public/SystemsWorld.h"

#include "Systems_Workspace/Life/AI/Reasoner/UtilityAiMemory.h"
#include "Systems_Workspace/Life/Components/AnimalComponent.h"

float UUtilityAiDistanceToConsideration::Score(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory)
{
	const auto Position = Actor->GetActorLocation();
	const auto* OtherActor = Memory.Access(this->ActorMemoryProperty).CastObject<AActor>();

	if (IsValid(OtherActor))
	{
		const auto OtherPosition = OtherActor->GetActorLocation();
		return FVector::Distance(Position, OtherPosition);
	}
	return INFINITY;
}
