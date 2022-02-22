#include "Life/AI/Considerations/UtilityAiMinConsideration.h"

#include "Systems/Public/Iterator.h"

float UUtilityAiMinConsideration::Score(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory)
{
	return IterStd(this->Considerations)
		.Fold<float>(INFINITY,
			[&](const auto Accum, auto& Consideration)
			{
				const auto Score = Consideration.Score(Actor, Systems, Memory);
				return FMath::Min(Accum, Score);
			});
}
