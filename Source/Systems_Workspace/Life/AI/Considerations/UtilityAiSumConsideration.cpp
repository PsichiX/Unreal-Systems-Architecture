#include "Systems_Workspace/Life/AI/Considerations/UtilityAiSumConsideration.h"

#include "Systems/Public/Iterator.h"

float UUtilityAiSumConsideration::Score(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory)
{
	return IterStd(this->Considerations)
		.Fold<float>(0,
			[&](const auto Accum, auto& Consideration)
			{
				const auto Score = Consideration.Score(Actor, Systems, Memory);
				return Accum + Score;
			});
}
