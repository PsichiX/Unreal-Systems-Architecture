#include "Life/AI/Reasoner/UtilityAiReasoner.h"

#include "Systems/Public/SystemsWorld.h"

#include "Life/AI/Reasoner/UtilityAiBehavior.h"
#include "Life/AI/Reasoner/UtilityAiMemory.h"
#include "Life/AI/Reasoner/UtilityAiPerception.h"
#include "Life/AI/Reasoner/UtilityAiTask.h"

FUtilityAiReasonerState::FUtilityAiReasonerState() : bActiveState(true), Consideration(), Task(Task)
{
}

FUtilityAiReasonerState::FUtilityAiReasonerState(FUtilityAiMappedConsideration InConsideration,
	UUtilityAiTask* InTask,
	bool bInActiveState)
	: bActiveState(bInActiveState), Consideration(InConsideration), Task(InTask)
{
}

UUtilityAiReasoner* UUtilityAiReasoner::FromBehavior(UObject* Owner, UUtilityAiBehavior* Behavior)
{
	if (IsValid(Behavior))
	{
		auto* Result = NewObject<UUtilityAiReasoner>(Owner, UUtilityAiReasoner::StaticClass());
		if (IsValid(Result))
		{
			Result->Perceptions = Behavior->Perceptions;
			Result->States.Reserve(Behavior->States.Num());
			for (const auto& Pair : Behavior->States)
			{
				const auto State = Pair.Value.Instantiate(Owner);
				if (State.IsSet())
				{
					Result->States.Add(Pair.Key, State.GetValue());
				}
			}
			return Result;
		}
	}
	return nullptr;
}

bool UUtilityAiReasoner::ChangeActiveState(AActor* Actor,
	FName Name,
	USystemsWorld& Systems,
	FUtilityAiMemory& Memory,
	bool bForced)
{
	if (this->ActiveState.IsNone() == false && Name != this->ActiveState)
	{
		if (auto* State = this->States.Find(this->ActiveState))
		{
			if (IsValid(State->Task) == false ||
				(bForced == false && State->Task->IsLocked(Actor, Systems)))
			{
				return false;
			}
			State->Task->OnExit(Actor, Systems, Memory);
		}
	}
	if (Name.IsNone() == false)
	{
		if (auto* State = this->States.Find(Name))
		{
			if (IsValid(State->Task))
			{
				if (State->Task->OnEnter(Actor, Systems, Memory, Name != this->ActiveState) ==
					false)
				{
					this->ActiveState = FName();
					return true;
				}
			}
		}
	}

	this->ActiveState = Name;
	return true;
}

bool UUtilityAiReasoner::Decide(AActor* Actor, USystemsWorld& Systems)
{
	if (this->States.Num() <= 0)
	{
		return false;
	}

	auto Memory = FUtilityAiMemory();
	for (auto* Perception : this->Perceptions)
	{
		if (IsValid(Perception))
		{
			Perception->Perceive(Actor, Systems, Memory);
		}
	}

	const auto Found =
		IterStd(this->States)
			.Filter([](const auto& Pair) { return Pair.Value.bActiveState; })
			.Map<TTuple<FName, float>>(
				[&](auto& Pair)
				{
					const auto Value = Pair.Value.Consideration.Score(Actor, Systems, Memory);
					return MakeTuple(Pair.Key, Value);
				})
			.ComparedBy([](const auto& A, const auto& B) { return A.Get<1>() > B.Get<1>(); });

	return Found.IsSet() &&
		ChangeActiveState(Actor, Found.GetValue().Get<0>(), Systems, Memory, true);
}
