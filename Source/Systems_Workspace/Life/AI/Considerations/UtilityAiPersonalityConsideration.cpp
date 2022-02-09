#include "Systems_Workspace/Life/AI/Considerations/UtilityAiPersonalityConsideration.h"

#include "Systems/Public/SystemsWorld.h"

float UUtilityAiPersonalityConsideration::Score(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory)
{
	const auto* Personality = Systems.Component<UPersonalityComponent>(Actor);
	return IsValid(Personality) ? Personality->Scales.Get(this->Kind) - TargetValue : 0;
}
