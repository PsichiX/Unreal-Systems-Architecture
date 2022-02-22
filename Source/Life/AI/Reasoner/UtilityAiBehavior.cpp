#include "Life/AI/Reasoner/UtilityAiBehavior.h"

#include "Life/AI/Reasoner/UtilityAiReasoner.h"
#include "Life/AI/Reasoner/UtilityAiTask.h"

TOptional<FUtilityAiReasonerState> FUtilityAiBehaviorState::Instantiate(UObject* Owner) const
{
	auto NewConsideration = FUtilityAiMappedConsideration(this->Consideration);
	NewConsideration.Consideration = DuplicateObject(NewConsideration.Consideration, Owner);
	auto* NewTask = DuplicateObject(this->Task, Owner);
	if (IsValid(NewConsideration.Consideration) && IsValid(NewTask))
	{
		return TOptional<FUtilityAiReasonerState>(
			FUtilityAiReasonerState(NewConsideration, NewTask, this->bActiveState));
	}
	return TOptional<FUtilityAiReasonerState>();
}
