#include "Systems_Workspace/Life/AI/Considerations/UtilityAiMaxConsideration.h"

#include "Systems/Public/Iterator.h"

float UUtilityAiMaxConsideration::Score(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory)
{
	return IterStd(this->Considerations)
		.Fold<float>(-INFINITY,
			[&](const auto Accum, auto& Consideration)
			{
				const auto Score = Consideration.Score(Actor, Systems, Memory);
				return FMath::Max(Accum, Score);
			});
}
