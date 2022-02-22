#include "Life/AI/Reasoner/UtilityAiConsideration.h"

#include "Life/AI/Reasoner/UtilityAiMemory.h"

float FUtilityAiMappedConsideration::Score(AActor* Actor,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory)
{
	const auto Value =
		IsValid(this->Consideration) ? this->Consideration->Score(Actor, Systems, Memory) : 0;
	const auto* Curve = this->ScoreMapping.GetRichCurveConst();
	return Curve != nullptr && Curve->IsEmpty() == false ? Curve->Eval(Value) : Value;
}
