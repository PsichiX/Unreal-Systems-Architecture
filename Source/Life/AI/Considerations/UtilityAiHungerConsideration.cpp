#include "Life/AI/Considerations/UtilityAiHungerConsideration.h"

#include "Shared/Components/HungerComponent.h"
#include "Systems/Public/SystemsWorld.h"

float UUtilityAiHungerConsideration::Score(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory)
{
	const auto* Hunger = Systems.Component<UHungerComponent>(Actor);
	return IsValid(Hunger) ? Hunger->Value - this->Threshold : 0;
}
