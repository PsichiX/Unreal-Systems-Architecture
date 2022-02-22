#include "Life/AI/Considerations/UtilityAiProductConsideration.h"

#include "Systems/Public/Iterator.h"

float UUtilityAiProductConsideration::Score(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory)
{
	if (this->Considerations.Num() <= 0)
	{
		return 0;
	}

	return IterStd(this->Considerations)
		.Fold<float>(1,
			[&](const auto Accum, auto& Consideration)
			{
				const auto Score = Consideration.Score(Actor, Systems, Memory);
				return Accum * Score;
			});
}
